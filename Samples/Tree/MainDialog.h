#pragma once

#include <Dialog.h>

class MainDialog : public Dialog
{
public:
    MainDialog();

protected:
    bool OnCreated() override;
};