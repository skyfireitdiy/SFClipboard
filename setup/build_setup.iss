; 脚本由 Inno Setup 脚本向导 生成！
; 有关创建 Inno Setup 脚本文件的详细资料请查阅帮助文档！

#define MyAppName "SFClipboard"
#define MyAppVersion "3.3.0.13"
#define MyAppPublisher "天火软件"
#define MyAppExeName "SFClipboard.exe"
#define BaseDir ".."

[Setup]
; 注: AppId的值为单独标识该应用程序。
; 不要为其他安装程序使用相同的AppId值。
; (生成新的GUID，点击 工具|在IDE中生成GUID。)
AppId={{EECD2A84-7F2C-4812-BDF5-CC8D45212447}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
DefaultDirName={pf}\{#MyAppName}
DisableProgramGroupPage=yes
LicenseFile={#BaseDir}\bin\licence.txt
InfoBeforeFile={#BaseDir}\bin\information_before.txt
InfoAfterFile={#BaseDir}\bin\information_after.txt
OutputDir={#BaseDir}\setup
OutputBaseFilename={#MyAppName}_{#MyAppVersion}_setup
SetupIconFile={#BaseDir}\icon.ico
Compression=lzma
SolidCompression=yes

[Languages]
Name: "chinesesimp"; MessagesFile: "compiler:Default.isl"
Name: "armenian"; MessagesFile: "compiler:Languages\Armenian.islu"
Name: "brazilianportuguese"; MessagesFile: "compiler:Languages\BrazilianPortuguese.isl"
Name: "catalan"; MessagesFile: "compiler:Languages\Catalan.isl"
Name: "chinesetrad"; MessagesFile: "compiler:Languages\ChineseTrad.isl"
Name: "corsican"; MessagesFile: "compiler:Languages\Corsican.isl"
Name: "czech"; MessagesFile: "compiler:Languages\Czech.isl"
Name: "danish"; MessagesFile: "compiler:Languages\Danish.isl"
Name: "dutch"; MessagesFile: "compiler:Languages\Dutch.isl"
Name: "english"; MessagesFile: "compiler:Languages\English.isl"
Name: "finnish"; MessagesFile: "compiler:Languages\Finnish.isl"
Name: "french"; MessagesFile: "compiler:Languages\French.isl"
Name: "german"; MessagesFile: "compiler:Languages\German.isl"
Name: "greek"; MessagesFile: "compiler:Languages\Greek.isl"
Name: "hebrew"; MessagesFile: "compiler:Languages\Hebrew.isl"
Name: "hungarian"; MessagesFile: "compiler:Languages\Hungarian.isl"
Name: "italian"; MessagesFile: "compiler:Languages\Italian.isl"
Name: "japanese"; MessagesFile: "compiler:Languages\Japanese.isl"
Name: "nepali"; MessagesFile: "compiler:Languages\Nepali.islu"
Name: "norwegian"; MessagesFile: "compiler:Languages\Norwegian.isl"
Name: "polish"; MessagesFile: "compiler:Languages\Polish.isl"
Name: "portuguese"; MessagesFile: "compiler:Languages\Portuguese.isl"
Name: "russian"; MessagesFile: "compiler:Languages\Russian.isl"
Name: "scottishgaelic"; MessagesFile: "compiler:Languages\ScottishGaelic.isl"
Name: "serbiancyrillic"; MessagesFile: "compiler:Languages\SerbianCyrillic.isl"
Name: "serbianlatin"; MessagesFile: "compiler:Languages\SerbianLatin.isl"
Name: "slovenian"; MessagesFile: "compiler:Languages\Slovenian.isl"
Name: "spanish"; MessagesFile: "compiler:Languages\Spanish.isl"
Name: "turkish"; MessagesFile: "compiler:Languages\Turkish.isl"
Name: "ukrainian"; MessagesFile: "compiler:Languages\Ukrainian.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked;
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "{#BaseDir}\bin\SFClipboard.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#BaseDir}\bin\D3Dcompiler_47.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#BaseDir}\bin\information_after.txt"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#BaseDir}\bin\information_before.txt"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#BaseDir}\bin\lang.ini"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#BaseDir}\bin\libEGL.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#BaseDir}\bin\libgcc_s_dw2-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#BaseDir}\bin\libGLESV2.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#BaseDir}\bin\libstdc++-6.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#BaseDir}\bin\libwinpthread-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#BaseDir}\bin\licence.txt"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#BaseDir}\bin\opengl32sw.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#BaseDir}\bin\Qt5Core.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#BaseDir}\bin\Qt5Gui.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#BaseDir}\bin\Qt5Sql.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#BaseDir}\bin\Qt5Svg.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#BaseDir}\bin\Qt5Widgets.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#BaseDir}\bin\SFClipboard.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#BaseDir}\bin\iconengines\*"; DestDir: "{app}\iconengines"; Flags: ignoreversion
Source: "{#BaseDir}\bin\imageformats\*"; DestDir: "{app}\imageformats"; Flags: ignoreversion
Source: "{#BaseDir}\bin\platforms\*"; DestDir: "{app}\platforms"; Flags: ignoreversion
Source: "{#BaseDir}\bin\sqldrivers\*"; DestDir: "{app}\sqldrivers"; Flags: ignoreversion
Source: "{#BaseDir}\bin\translations\*"; DestDir: "{app}\translations"; Flags: ignoreversion


; 注意: 不要在任何共享系统文件上使用“Flags: ignoreversion”

[Icons]
Name: "{commonprograms}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: quicklaunchicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

