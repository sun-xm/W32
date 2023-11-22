#pragma once

#include <Dialog.h>
#include <Font.h>
#include <StatusBar.h>

class AppDialog : public Dialog
{
public:
    AppDialog();

protected:
    bool OnCreated() override;
    void OnSize() override;

private:
    bool CreateStatus();
    void Position(int pos);

private:
    StatusBar status;
    Font font;
};