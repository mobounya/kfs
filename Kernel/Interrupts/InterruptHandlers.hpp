#pragma once

extern "C"
{
    void DB_fault(void);
    void NMI_interrupt(void);
    void BP_trap(void);
    void UD_fault(void);
    void NM_fault(void);
    void DF_abort(void);
    void TS_fault(void);
    void NP_fault(void);
    void SS_fault(void);
    void GP_fault(void);
    void PF_fault(void);
    void MF_fault(void);
    void AC_fault(void);
    void MC_fault(void);
    void XM_fault(void);
    void keyboard_handler(void);
    void default_handler(void);
}