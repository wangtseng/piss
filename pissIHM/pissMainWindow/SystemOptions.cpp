#include "SystemOptions.h"


/**
 * @brief SystemOptions::SystemOptions
 */
SystemOptions::SystemOptions()
{
    this->initVariable();
    this->constructIHM();
    this->setConnections();
}

//! ------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SystemOptions::initVariable
//!
void SystemOptions::initVariable(){
    comboBoxStyle = "QComboBox {border: 0px solid gray;border-radius: 3px;padding: 1px 18px 1px 3px;min-width: 6em;}"
                    "QComboBox::drop-down { subcontrol-origin: padding;subcontrol-position: top right;width: 15px;border-left-width: 1px; border-left-color: lightGray; border-left-style: solid;border-top-right-radius: 3px; border-bottom-right-radius: 3px;}"
                    "QComboBox::down-arrow {border-image: url(:/down_arrow.png); } "
                    "QComboBox::down-arrow:on {top: 1px; left: 1px;}";
}

//! ------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SystemOptions::constructIHM
//!
void SystemOptions::constructIHM(){
    systemOptionsLayout = new QGridLayout(this);

    systemBackgroundColorOptionsLabel = new QLabel("System background color: ");

    systemBackgroundColorOptions = new QComboBox();
    fillColorList(systemBackgroundColorOptions);
    systemBackgroundColorOptions->setStyleSheet(comboBoxStyle);

    systemWorkSpaceColorOptionsLabel = new QLabel("System workspace color: ");

    systemWorkSpaceColorOptions = new QComboBox();
    fillColorList(systemWorkSpaceColorOptions);
    systemWorkSpaceColorOptions->setStyleSheet(comboBoxStyle);

    confirmButton = new QPushButton("ok");
    cancelButton = new QPushButton("cancel");

    systemOptionsLayout->addWidget(systemBackgroundColorOptionsLabel, 0, 0);
    systemOptionsLayout->addWidget(systemBackgroundColorOptions, 0, 1);
    systemOptionsLayout->addWidget(systemWorkSpaceColorOptionsLabel, 1, 0);
    systemOptionsLayout->addWidget(systemWorkSpaceColorOptions, 1, 1);
    systemOptionsLayout->addWidget(confirmButton, 2, 0);
    systemOptionsLayout->addWidget(cancelButton, 2, 1);
}

//! ------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SystemOptions::setConnections
//!
void SystemOptions::setConnections(){
    this->connect(confirmButton, SIGNAL(clicked()), this, SLOT(onConfirmButtonClicked()));
    this->connect(cancelButton, SIGNAL(clicked()), this, SLOT(onCancelButtonClicked()));
    this->connect(systemBackgroundColorOptions, SIGNAL(currentIndexChanged(int)), this,  SLOT(systemColorOptionChanged()));
    this->connect(systemWorkSpaceColorOptions, SIGNAL(currentIndexChanged(int)), this, SLOT(workspaceColorOptionChanged()));
}

//! ------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SystemOptions::getSystemGlobalBackgroundColor
//! \return
//!
QString SystemOptions::getSystemGlobalBackgroundColor(){
    return this->systemGlobalBackgroundColor;
}

//! ------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SystemOptions::getSystemGlobalWorkSpaceColor
//! \return
//!
QString SystemOptions::getSystemGlobalWorkSpaceColor(){
    return this->systemGlobalWorkSpaceColor;
}

//! ------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SystemOptions::systemColorOptionChanged
//!
void SystemOptions::systemColorOptionChanged(){
    systemGlobalBackgroundColor = systemBackgroundColorOptions->currentText();
}

//! ------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SystemOptions::workspaceColorOptionChanged
//!
void SystemOptions::workspaceColorOptionChanged(){
    systemGlobalWorkSpaceColor = systemWorkSpaceColorOptions->currentText();
}

//! ------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SystemOptions::onConfirmButtonClicked
//!
void SystemOptions::onConfirmButtonClicked(){
    emit confirm();
    this->close();
}

//! ------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SystemOptions::onCancelButtonClicked
//!
void SystemOptions::onCancelButtonClicked(){
    emit cancel();
    this->close();
}

//! ------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SystemOptions::fillColorList
//! \param systemColorOptions
//!
void SystemOptions::fillColorList(QComboBox *systemColorOptions){
    QStringList color_list = QColor::colorNames();
    for(int cpt = 0; cpt < color_list.size(); cpt++){
        QPixmap *pix = new QPixmap(QSize(40, 15));
        pix->fill(QColor(color_list.at(cpt)));
        systemColorOptions->addItem(QIcon(*pix),color_list.at(cpt));
        systemColorOptions->setIconSize(QSize(40,15));
        systemColorOptions->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    }
}
