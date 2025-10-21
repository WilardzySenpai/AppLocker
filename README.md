================================================================================
                              APP LOCKER v1.0.0
================================================================================

Thank you for installing App Locker!

--------------------------------------------------------------------------------
WHAT IS APP LOCKER?
--------------------------------------------------------------------------------

App Locker is a Windows application that allows you to password-protect other
applications on your computer. When a locked app tries to run, it will be
blocked until the correct password is entered.

--------------------------------------------------------------------------------
SYSTEM REQUIREMENTS
--------------------------------------------------------------------------------

- Windows 10 or Windows 11
- Administrator privileges
- Approximately 50 MB of disk space

--------------------------------------------------------------------------------
FIRST-TIME SETUP
--------------------------------------------------------------------------------

1. Launch App Locker for the first time
2. You will be prompted to create a MASTER PASSWORD
3. Remember this password - you'll need it to:
   - Access the App Locker interface
   - Lock/unlock applications
   - Change settings
   - Exit the program

IMPORTANT: If you forget your master password, you will need to delete the
file "master_password.dat" from the installation directory and start over.

--------------------------------------------------------------------------------
HOW TO USE
--------------------------------------------------------------------------------

LOCKING AN APPLICATION:
1. Enter the application's executable name (e.g., "chrome.exe", "Discord.exe")
2. Enter a password specifically for that app
3. Enter your master password
4. Click "Lock"

UNLOCKING AN APPLICATION:
1. Enter the application's executable name
2. Enter your master password
3. Click "Unlock"

WHEN A LOCKED APP IS LAUNCHED:
- App Locker will automatically detect it
- The app will be closed immediately
- A password prompt will appear
- Enter the app's password (NOT the master password)
- If correct, the app will be launched
- If incorrect, access is denied

--------------------------------------------------------------------------------
SYSTEM TRAY FEATURES
--------------------------------------------------------------------------------

App Locker runs in the background and appears in your system tray.

Right-click the tray icon for options:
- Show: Opens the main window (requires master password)
- Change Master Password: Update your master password
- Exit: Close App Locker (requires master password)

Closing the main window minimizes App Locker to the tray.

--------------------------------------------------------------------------------
SECURITY NOTES
--------------------------------------------------------------------------------

IMPORTANT - PLEASE READ:

This software provides BASIC application locking for personal use. It is NOT:
- Enterprise-grade security software
- Protection against determined attackers
- A substitute for Windows user account controls
- Unbreakable security

Ways this protection can be bypassed:
- Booting in Safe Mode
- Using Task Manager with administrator rights
- Deleting the password files
- Ending the App Locker process
- Accessing files directly without launching the app

USE CASES:
✓ Parental controls (basic level)
✓ Privacy from casual users
✓ Personal reminders not to open certain apps
✓ Shared computer basic protection

NOT SUITABLE FOR:
✗ Corporate/business security
✗ Protecting sensitive data
✗ Preventing tech-savvy users
✗ Critical security requirements

--------------------------------------------------------------------------------
TIPS & BEST PRACTICES
--------------------------------------------------------------------------------

1. Use strong passwords (at least 8 characters)
2. Don't use the same password for multiple apps
3. Keep your master password safe and memorable
4. Test locking on non-critical apps first
5. Remember: App names must include .exe extension
6. Common app names:
   - Chrome: chrome.exe
   - Discord: Discord.exe
   - Steam: steam.exe
   - Spotify: Spotify.exe

To find an app's executable name:
- Open Task Manager (Ctrl+Shift+Esc)
- Go to Details tab
- Look for the app's .exe name

--------------------------------------------------------------------------------
TROUBLESHOOTING
--------------------------------------------------------------------------------

Q: I forgot my master password!
A: Navigate to the installation folder and delete "master_password.dat"
   You'll need to set up a new master password and re-lock your apps.

Q: App Locker isn't detecting my locked app!
A: Make sure you entered the exact .exe name (case-insensitive but must
   include .exe). Check Task Manager > Details for the correct name.

Q: The locked app keeps running!
A: App Locker needs to be running in the background. Check your system tray.
   Also, you need administrator privileges for App Locker to work properly.

Q: Can I lock folders or files?
A: No, App Locker only locks executable applications (.exe files).

Q: How do I uninstall?
A: Use Windows Settings > Apps > App Locker > Uninstall
   Or run the uninstaller from the Start Menu.

--------------------------------------------------------------------------------
FILES CREATED
--------------------------------------------------------------------------------

App Locker creates these files in its installation directory:
- locked_apps.txt: List of locked applications
- master_password.dat: Your encrypted master password
- app_passwords.dat: Encrypted passwords for each app

These files are automatically deleted when you uninstall App Locker.

--------------------------------------------------------------------------------
SUPPORT & FEEDBACK
--------------------------------------------------------------------------------

For issues, suggestions, or questions:
- GitHub: https://github.com/WilardzySenpai/AppLocker
- Email: hoshinohachiki@gmail.com

--------------------------------------------------------------------------------
VERSION HISTORY
--------------------------------------------------------------------------------

Version 1.0.0 (Current)
- Initial release
- Master password protection
- Individual app passwords
- System tray integration
- Background monitoring

--------------------------------------------------------------------------------
LICENSE
--------------------------------------------------------------------------------

See LICENSE.txt for license information.

================================================================================
                        Thank you for using App Locker!
================================================================================