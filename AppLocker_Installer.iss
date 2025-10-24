; App Locker Installer Script
; Created with Inno Setup

#define MyAppName "App Locker"
#define MyAppVersion "1.0.3"
#define MyAppPublisher "Hachiki"
#define MyAppURL "https://github.com/WilardzySenpai/AppLocker"
#define MyAppExeName "AppLocker.exe"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Generate a new GUID using Tools > Generate GUID in Inno Setup
AppId={{42AE2E53-EDCE-4EC0-B107-398699B1DC52}}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={autopf}\{#MyAppName}
DefaultGroupName={#MyAppName}
AllowNoIcons=yes
LicenseFile=C:\Users\Hp\OneDrive\Pictures\Desktop\AppLocker\AppLockerInstaller\LICENSE.txt
InfoBeforeFile=C:\Users\Hp\OneDrive\Pictures\Desktop\AppLocker\AppLockerInstaller\README.txt
OutputDir=C:\Users\Hp\OneDrive\Pictures\Desktop\AppLocker\AppLockerInstaller\Installer
OutputBaseFilename=AppLocker_Setup_v{#MyAppVersion}
Compression=lzma
SolidCompression=yes
WizardStyle=modern
PrivilegesRequired=admin
DisableDirPage=auto
DisableProgramGroupPage=auto
SetupIconFile=C:\Users\Hp\OneDrive\Pictures\Desktop\AppLocker\AppLockerInstaller\icon.ico
UninstallDisplayIcon={app}\{#MyAppExeName}

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "startupicon"; Description: "Run App Locker at Windows startup"; GroupDescription: "Startup Options:"; Flags: unchecked

[Files]
; Main executable
Source: "C:\Users\Hp\OneDrive\Pictures\Desktop\AppLocker\AppLockerRelease\{#MyAppExeName}"; DestDir: "{app}"; Flags: ignoreversion

; Qt DLLs and dependencies (windeployqt output)
Source: "C:\Users\Hp\OneDrive\Pictures\Desktop\AppLocker\AppLockerRelease\*.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\Hp\OneDrive\Pictures\Desktop\AppLocker\AppLockerRelease\platforms\*"; DestDir: "{app}\platforms"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:\Users\Hp\OneDrive\Pictures\Desktop\AppLocker\AppLockerRelease\styles\*"; DestDir: "{app}\styles"; Flags: ignoreversion recursesubdirs createallsubdirs

; CRITICAL FIX: Include icon files in the application directory as a fallback
Source: "C:\Users\Hp\OneDrive\Pictures\Desktop\AppLocker\AppLockerInstaller\logo\icon.ico"; DestDir: "{app}\logo"; Flags: ignoreversion
Source: "C:\Users\Hp\OneDrive\Pictures\Desktop\AppLocker\AppLockerInstaller\logo\icon_16x.png"; DestDir: "{app}\logo"; Flags: ignoreversion
Source: "C:\Users\Hp\OneDrive\Pictures\Desktop\AppLocker\AppLockerInstaller\logo\icon_32x.png"; DestDir: "{app}\logo"; Flags: ignoreversion
; Source: "C:\Users\Hp\OneDrive\Pictures\Desktop\AppLocker\AppLockerInstaller\logo\icon_48x.png"; DestDir: "{app}\logo"; Flags: ignoreversion

; Documentation
Source: "C:\Users\Hp\OneDrive\Pictures\Desktop\AppLocker\AppLockerInstaller\README.txt"; DestDir: "{app}"; Flags: ignoreversion isreadme
Source: "C:\Users\Hp\OneDrive\Pictures\Desktop\AppLocker\AppLockerInstaller\LICENSE.txt"; DestDir: "{app}"; Flags: ignoreversion

; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon
Name: "{commonstartup}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: startupicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

[UninstallDelete]
Type: files; Name: "{app}\locked_apps.txt"
Type: files; Name: "{app}\master_password.dat"
Type: files; Name: "{app}\app_passwords.dat"

[Code]
procedure InitializeWizard;
begin
  WizardForm.LicenseAcceptedRadio.Checked := False;
end;

function InitializeSetup(): Boolean;
begin
  Result := True;
  if not IsAdmin then
  begin
    MsgBox('App Locker requires administrator privileges to install and run properly.' + #13#10 + #13#10 + 
           'Please run the installer as Administrator.', mbError, MB_OK);
    Result := False;
  end;
end;