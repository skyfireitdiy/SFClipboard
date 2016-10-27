; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "SFClipboard"
#define MyAppVersion "2.1"
#define MyAppPublisher "SkyFire, Inc."
#define MyAppURL "http://git.oschina.net/skyfireitdiy/SFClipboard"
#define MyAppExeName "SFClipboard.exe"
#define ProjectDir ".."

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{D157BAE7-CACB-4460-B900-B9B30CA58C4D}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\{#MyAppName}
DisableProgramGroupPage=yes
LicenseFile={#ProjectDir}\install\licence.txt
InfoBeforeFile={#ProjectDir}\install\information_before.txt
InfoAfterFile={#ProjectDir}\install\information_after.txt
OutputDir={#ProjectDir}\install
OutputBaseFilename=SFClipboard_{#MyAppVersion}_setup
SetupIconFile={#ProjectDir}\install\icon.ico
Password=
Compression=lzma
SolidCompression=yes
PrivilegesRequired=admin

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked; OnlyBelowVersion: 0,6.1

[Files]
Source: "{#ProjectDir}\bin\SFClipboard.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#ProjectDir}\bin\libgcc_s_dw2-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#ProjectDir}\bin\libstdc++-6.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#ProjectDir}\bin\libwinpthread-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#ProjectDir}\bin\Qt5Core.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#ProjectDir}\bin\Qt5Gui.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#ProjectDir}\bin\Qt5Widgets.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#ProjectDir}\bin\SFClipboard.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#ProjectDir}\bin\platforms\qwindows.dll"; DestDir: "{app}\platforms"; Flags: ignoreversion recursesubdirs createallsubdirs
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{commonprograms}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: quicklaunchicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

