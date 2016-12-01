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


bool set_button_backimg(QPushButton *button, QString img_path){
    QPixmap pixmap(img_path);
    if(pixmap.isNull())
        return false;
    if(button==nullptr)
        return false;
    button->setFixedSize(pixmap.size());
    button->setMask(pixmap.createHeuristicMask());
    button->setIcon(pixmap);
    button->setIconSize(pixmap.size());
    return true;
}
