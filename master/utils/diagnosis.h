#ifndef DIAGNOSIS_H
#define DIAGNOSIS_H

typedef enum HftError_t
{
    NoError = 0,
    NotEnoughMoneyErr = -1,
    NetworkError = -2,
    InvalidArgsErr = -3,
    InvlaidOperErr = -4,
    TimeNotReachedErr = -5,
    TimeExpiredErr = -6,
    IOReadErr = -7,
    IOWriteErr = -8,
    InvalidUserNameOrPasswdErr = -9,
    InvalidNetAddrErr = -10,
    UnknownError = -11
} HftError;


#define DEF_LOGFOLDER "/home/harry/logs"

void RedirectLog2Files();

#endif // DIAGNOSIS_H
