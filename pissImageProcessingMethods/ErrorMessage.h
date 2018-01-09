#ifndef ERRORMESSAGE_H
#define ERRORMESSAGE_H

//! error code to indicate the cause of the error
typedef enum{
    PROCESSING_NO_ERRROR = 0,
    PROCESSING_DERIVATION_ERROR,
    PROCESSING_CONVOLUTION_ERROR
}eProcessingErrorCode;

class ErrorMessage
{
public:
    ErrorMessage();
};

#endif // ERRORMESSAGE_H
