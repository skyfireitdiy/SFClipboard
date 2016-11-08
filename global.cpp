#include <Global.h>

#ifdef __WIN32

BOOL IsRunasAdmin()
{
    BOOL bElevated = FALSE;
    HANDLE hToken = NULL;
    if ( !OpenProcessToken( GetCurrentProcess(), TOKEN_QUERY, &hToken ) )
        return FALSE;

    TOKEN_ELEVATION tokenEle;
    DWORD dwRetLen = 0;

    // Retrieve token elevation information
    if ( GetTokenInformation( hToken, TokenElevation, &tokenEle, sizeof(tokenEle), &dwRetLen ) )
    {
        if ( dwRetLen == sizeof(tokenEle) )
        {
            bElevated = tokenEle.TokenIsElevated;
        }
    }
    CloseHandle( hToken );
    return bElevated;
}

#endif
