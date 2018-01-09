#include "Omega3.h"


void Omega3::AcquisitionTask::run(){
    while(1){

        //! qDebug()<< "try to open the first available device";
        if (drdOpen () < 0) {
            //qDebug()<<"no device available...";
            //dhdSleep (2.0);
            omegaDetected = false;
        }
        else{
            omegaDetected = true;
        }

        if(omegaDetected){
            break;
        }

        sleep(1);
    }

    done  = 0;
    double p[DHD_MAX_DOF];
    double r[3][3];
    double v[DHD_MAX_DOF];
    double f[DHD_MAX_DOF];
    double normf, normt;

    double t0    = dhdGetTime ();
    double t1    = t0;
    bool   base  = false;
    bool   wrist = false;
    bool   grip  = false;

    //! Eigen objects (mapped to the arrays above)
    Map<Vector3d> position(&p[0], 3);
    Map<Vector3d> force   (&f[0], 3);
    Map<Vector3d> torque  (&f[3], 3);
    Map<Vector3d> velpos  (&v[0], 3);
    Map<Vector3d> velrot  (&v[3], 3);
    Matrix3d center;
    Matrix3d rotation;

    //! center of workspace
    center.setIdentity ();                           // rotation (matrix)
    double nullPose[DHD_MAX_DOF] = { 0.0, 0.0, 0.0,  // translation
                                     0.0, 0.0, 0.0,  // rotation (joint angles)
                                     0.0 };          // gripper

    //! print out device identifier
    if (!drdIsSupported ()) {
      dhdSleep (2.0);
      drdClose ();
    }

    //! perform auto-initialization
    if (!drdIsInitialized () && drdAutoInit () < 0) {
      qDebug()<<"error: auto-initialization failed"<<dhdErrorGetLastStr ();
      dhdSleep (2.0);
    }
    else if (drdStart () < 0) {
      printf ("error: regulation thread failed to start (%s)\n", dhdErrorGetLastStr ());
      dhdSleep (2.0);
    }

    //! move to center
    drdMoveTo (nullPose);

    // request a null force (only gravity compensation will be applied)
    // this will only apply to unregulated axis
    for (int i=0; i<DHD_MAX_DOF; i++) f[i] = 0.0;
    drdSetForceAndTorqueAndGripperForce (f);

    // disable all axis regulation (but leave regulation thread running)
    drdRegulatePos  (base);
    drdRegulateRot  (wrist);
    drdRegulateGrip (grip);

    //! save current pos
    drdGetPositionAndOrientation (p, r);
    for (int i=0; i<3; i++) rotation.row(i) = Vector3d::Map(&r[i][0], 3);

    this->omega->patientHandling->setOmegaPosition(p[0],p[1],p[2]);

    long long cpt = 0;

    // loop while the button is not pushed
    while (!done) {

      // synchronize with regulation thread
      drdWaitForTick ();

      // get position/orientation/gripper and update Eigen rotation matrix
      drdGetPositionAndOrientation (p, r);
      for (int i=0; i<3; i++) rotation.row(i) = Vector3d::Map(&r[i][0], 3);

      if(cpt%31==0){
          this->omega->patientHandling->setOmegaPosition(p[0],p[1],p[2]);
      }
      else{
          cpt += 1;
      }

      // get position/orientation/gripper velocity
      drdGetVelocity (v);

      // compute base centering force
      force = - KP * position;

      // compute wrist centering torque
      AngleAxisd deltaRotation (rotation.transpose() * center);
      torque = rotation * (KR * deltaRotation.angle() * deltaRotation.axis());

      // compute gripper centering force
      f[6]  = - KG * (p[6] - 0.015);

      // scale force to a pre-defined ceiling
      if ((normf = force.norm()) > MAXF) force *= MAXF/normf;

      // scale torque to a pre-defined ceiling
      if ((normt = torque.norm()) > MAXT) torque *= MAXT/normt;

      // scale gripper force to a pre-defined ceiling
      if (f[6] >  MAXG) f[6] =  MAXG;
      if (f[6] < -MAXG) f[6] = -MAXG;

      // add damping
      force  -= KVP * velpos;
      torque -= KWR * velrot;
      f[6]   -= KVG * v[6];

      // apply centering force with damping
      if (drdSetForceAndTorqueAndGripperForce (f, -1) < DHD_NO_ERROR) {
        printf ("error: cannot set force (%s)\n", dhdErrorGetLastStr ());
        done = 1;
      }

      // local loop refresh rate
      //count++;

      // display refresh rate and position at 10Hz
      t1 = drdGetTime ();

      if ((t1-t0) > REFRESH_INTERVAL) {

        // retrieve/compute information to display
//        double freq = (double)count/(t1-t0)*1e-3;
//        count = 0;
//        t0 = t1;
//        qDebug()<<freq<<cpt;

        if (dhdGetButtonMask()) {
            qDebug()<<"stop";
            dhdClose ();
            done = 1;
        }

      }

      //qDebug()<<cpt<<t1;
      //dhdSleep (0.1);
    }
}

/**
 * @brief Omega3::Omega3
 */
Omega3::Omega3()
{

}

void Omega3::setPatientHandling(Patient *patientHandling){
    this->patientHandling = patientHandling;
}

//! -----------------------------------------------------------------------------------
//!
//! \brief Omega3::omegaAvailable
//! \return
//!
bool Omega3::omegaAvailable(){
    return acquisitionTask.omegaDetected;
}

//! -----------------------------------------------------------------------------------
//!
//! \brief Omega3::launch
//!
void Omega3::launch(){
    acquisitionTask.omega = this;
    acquisitionTask.start();
}

//! -----------------------------------------------------------------------------------
//!
//! \brief Omega3::terminate
//!
void Omega3::terminate(){

    if(this->omegaAvailable()){
        this->acquisitionTask.done = 1;
        // stop regulation
        drdStop ();

        // close the connection
        printf ("cleaning up...\n");
        drdClose ();
        // happily exit
        printf ("\ndone.\n");
    }
}
