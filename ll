./minishell 
minishell$ ..
minishell: ..: command not found
minishell$ .
minishell: ..: filename argument required.: usage: .                                            filename [arguments]
minishell$ ....
minishell: ....: command not found
minishell$ .........
minishell: .........: command not found
minishell$ .
minishell: ..: filename argument required.: usage: .                                            filename [arguments]
minishell$ .
minishell: ..: filename argument required.: usage: .                                            filename [arguments]
minishell$ .
minishell: ..: filename argument required.: usage: .                                            filename [arguments]
minishell$ .
minishell: ..: filename argument required.: usage: .                                            filename [arguments]
minishell$ .
minishell: ..: filename argument required.: usage: .                                            filename [arguments]
minishell$ .
minishell: ..: filename argument required.: usage: .                                            filename [arguments]
minishell$ .
minishell: ..: filename argument required.: usage: .                                            filename [arguments]
minishell$ .
minishell: ..: filename argument required.: usage: .                                            filename [arguments]
minishell$ .
minishell: ..: filename argument required.: usage: .                                            filename [arguments]
minishell$ 3.4
minishell: 3.4: command not found
minishell$ 3.4
minishell: 3.4: command not found
minishell$ .3
minishell: .3: command not found
minishell$ 4.
minishell: 4.: command not found
minishell$ .67
minishell: .67: command not found
minishell$ .
minishell: ..: filename argument required.: usage: .                                            filename [arguments]
minishell$ 8.9.9.8.08
minishell: 8.9.9.8.08: command not found
minishell$ 90
minishell: 90: command not found
minishell$ 89
minishell: 89: command not found
minishell$ 089.0
minishell: 089.0: command not found
minishell$ 
minishell$ elgpeiu9o
minishell: elgpeiu9o: command not found
minishell$ ls
README.md               file                    ls                      minishell.c             todo
_libft                  file*                   makefile                minishell.o             wildcard
env                     include                 massraybayoubmassrayb   parsing                 younes.txt
execution               ll                      minishell               test_comd.txt
minishell$ ls|ls\
minishell: ls\: command not found
minishell$ ls|ls
README.md               file                    ls                      minishell.c             todo
_libft                  file*                   makefile                minishell.o             wildcard
env                     include                 massraybayoubmassrayb   parsing                 younes.txt
execution               ll                      minishell               test_comd.txt
minishell$ 
minishell$ 
minishell$ 
minishell$ 
minishell$ 
minishell$ 
minishell$ 
minishell$ 
minishell$ export
declare -x COLORTERM="truecolor"
declare -x COMMAND_MODE="unix2003"
declare -x C_INCLUDE_PATH="/mnt/homes/massrayb/.brew/include:/mnt/homes/massrayb/.brew/include:"
declare -x GIT_ASKPASS="/Applications/Visual Studio Code.app/Contents/Resources/app/extensions/git/dist/askpass.sh"
declare -x HOME="/mnt/homes/massrayb"
declare -x LANG="en_US.UTF-8"
declare -x LD_LIBRARY_PATH="/mnt/homes/massrayb/.brew/lib:/mnt/homes/massrayb/.brew/lib:"
declare -x LESS="-R"
declare -x LIBRARY_PATH="/mnt/homes/massrayb/.brew/lib:/mnt/homes/massrayb/.brew/lib:"
declare -x LOGNAME="massrayb"
declare -x LSCOLORS="Gxfxcxdxbxegedabagacad"
declare -x LS_COLORS="di=1;36:ln=35:so=32:pi=33:ex=31:bd=34;46:cd=34;43:su=30;41:sg=30;46:tw=30;42:ow=30;43"
declare -x LaunchInstanceID="55005D54-8054-4A6C-A7F1-A0933F868973"
declare -x MallocNanoZone="0"
declare -x OLDPWD="/mnt/homes/massrayb/Desktop/minishell"
declare -x ORIGINAL_XDG_CURRENT_DESKTOP="undefined"
declare -x PAGER="less"
declare -x PATH="/mnt/homes/massrayb/.brew/bin:/mnt/homes/massrayb/goinfre/homebrew/bin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin:/usr/local/munki:/Library/Apple/usr/bin"
declare -x PWD="/mnt/homes/massrayb/Desktop/minishell"
declare -x SECURITYSESSIONID="186a8"
declare -x SHELL="/bin/zsh"
declare -x SHLVL="2"
declare -x SSH_AUTH_SOCK="/private/tmp/com.apple.launchd.BaT6qkyczg/Listeners"
declare -x TERM="xterm-256color"
declare -x TERM_PROGRAM="vscode"
declare -x TERM_PROGRAM_VERSION="1.89.1"
declare -x TMPDIR="/var/folders/zz/zyxvpxvq6csfxvn_n000cppm0035nn/T/"
declare -x USER="massrayb"
declare -x USER_ZDOTDIR="/mnt/homes/massrayb"
declare -x VSCODE_GIT_ASKPASS_EXTRA_ARGS
declare -x VSCODE_GIT_ASKPASS_MAIN="/Applications/Visual Studio Code.app/Contents/Resources/app/extensions/git/dist/askpass-main.js"
declare -x VSCODE_GIT_ASKPASS_NODE="/Applications/Visual Studio Code.app/Contents/Frameworks/Code Helper (Plugin).app/Contents/MacOS/Code Helper (Plugin)"
declare -x VSCODE_GIT_IPC_HANDLE="/var/folders/zz/zyxvpxvq6csfxvn_n000cppm0035nn/T/vscode-git-57f8d569cf.sock"
declare -x VSCODE_INJECTION="1"
declare -x XPC_FLAGS="0x0"
declare -x XPC_SERVICE_NAME="0"
declare -x ZDOTDIR="/mnt/homes/massrayb"
declare -x ZSH="/mnt/homes/massrayb/.oh-my-zsh"
declare -x __CF_USER_TEXT_ENCODING="0x196B5:0x0:0x0"
minishell$ env -i ./minishell
minishell: -i : No such file or directory
minishell$ ls -la
total 464
drwxr-xr-x  24 massrayb  2024_khouribga     816 Jul  7 22:46 .
drwxr-xr-x  32 massrayb  2024_khouribga    1088 Jul  6 14:02 ..
drwxr-xr-x  18 massrayb  2024_khouribga     612 Jul  7 22:19 .git
-rw-r--r--   1 massrayb  2024_khouribga       8 Jul  6 14:02 .gitignore
drwxr-xr-x   3 massrayb  2024_khouribga     102 Jul  6 15:19 .vscode
-rw-r--r--   1 massrayb  2024_khouribga       0 Jul  7 21:54 README.md
drwxr-xr-x  90 massrayb  2024_khouribga    3060 Jul  7 22:46 _libft
drwxr-xr-x  15 massrayb  2024_khouribga     510 Jul  7 22:46 env
drwxr-xr-x  15 massrayb  2024_khouribga     510 Jul  7 22:46 execution
-rw-r--r--   1 massrayb  2024_khouribga       0 Jul  7 21:20 file
-rw-r--r--   1 massrayb  2024_khouribga     246 Jul  7 21:20 file*
drwxr-xr-x   8 massrayb  2024_khouribga     272 Jul  7 22:17 include
-rw-r--r--   1 massrayb  2024_khouribga       0 Jul  7 21:20 ll
-rw-r--r--   1 massrayb  2024_khouribga      11 Jul  7 21:20 ls
-rw-r--r--   1 massrayb  2024_khouribga    6118 Jul  7 22:17 makefile
-rw-r--r--   1 massrayb  2024_khouribga    8595 Jul  6 14:02 massraybayoubmassrayb
-rwxr-xr-x   1 massrayb  2024_khouribga  134752 Jul  7 22:46 minishell
-rw-r--r--   1 massrayb  2024_khouribga    2936 Jul  7 22:46 minishell.c
-rw-r--r--   1 massrayb  2024_khouribga    4496 Jul  7 22:46 minishell.o
drwxr-xr-x  14 massrayb  2024_khouribga     476 Jul  7 22:46 parsing
-rw-r--r--   1 massrayb  2024_khouribga     427 Jul  6 14:02 test_comd.txt
-rw-r--r--   1 massrayb  2024_khouribga   47472 Jul  7 22:17 todo
drwxr-xr-x  16 massrayb  2024_khouribga     544 Jul  7 22:46 wildcard
-rw-r--r--   1 massrayb  2024_khouribga    1917 Jul  7 21:22 younes.txt
minishell$ env
SECURITYSESSIONID=186a8
USER=massrayb
MallocNanoZone=0
COMMAND_MODE=unix2003
PATH=/mnt/homes/massrayb/.brew/bin:/mnt/homes/massrayb/goinfre/homebrew/bin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin:/usr/local/munki:/Library/Apple/usr/bin
HOME=/mnt/homes/massrayb
SHELL=/bin/zsh
LaunchInstanceID=55005D54-8054-4A6C-A7F1-A0933F868973
__CF_USER_TEXT_ENCODING=0x196B5:0x0:0x0
XPC_SERVICE_NAME=0
SSH_AUTH_SOCK=/private/tmp/com.apple.launchd.BaT6qkyczg/Listeners
XPC_FLAGS=0x0
LOGNAME=massrayb
TMPDIR=/var/folders/zz/zyxvpxvq6csfxvn_n000cppm0035nn/T/
ORIGINAL_XDG_CURRENT_DESKTOP=undefined
SHLVL=2
PWD=/mnt/homes/massrayb/Desktop/minishell
OLDPWD=/mnt/homes/massrayb/Desktop/minishell
ZSH=/mnt/homes/massrayb/.oh-my-zsh
PAGER=less
LESS=-R
LSCOLORS=Gxfxcxdxbxegedabagacad
LS_COLORS=di=1;36:ln=35:so=32:pi=33:ex=31:bd=34;46:cd=34;43:su=30;41:sg=30;46:tw=30;42:ow=30;43
C_INCLUDE_PATH=/mnt/homes/massrayb/.brew/include:/mnt/homes/massrayb/.brew/include:
LIBRARY_PATH=/mnt/homes/massrayb/.brew/lib:/mnt/homes/massrayb/.brew/lib:
LD_LIBRARY_PATH=/mnt/homes/massrayb/.brew/lib:/mnt/homes/massrayb/.brew/lib:
TERM_PROGRAM=vscode
TERM_PROGRAM_VERSION=1.89.1
LANG=en_US.UTF-8
COLORTERM=truecolor
GIT_ASKPASS=/Applications/Visual Studio Code.app/Contents/Resources/app/extensions/git/dist/askpass.sh
VSCODE_GIT_ASKPASS_NODE=/Applications/Visual Studio Code.app/Contents/Frameworks/Code Helper (Plugin).app/Contents/MacOS/Code Helper (Plugin)
VSCODE_GIT_ASKPASS_MAIN=/Applications/Visual Studio Code.app/Contents/Resources/app/extensions/git/dist/askpass-main.js
VSCODE_GIT_IPC_HANDLE=/var/folders/zz/zyxvpxvq6csfxvn_n000cppm0035nn/T/vscode-git-57f8d569cf.sock
VSCODE_INJECTION=1
ZDOTDIR=/mnt/homes/massrayb
USER_ZDOTDIR=/mnt/homes/massrayb
TERM=xterm-256color
_=/mnt/homes/massrayb/Desktop/minishell/./minishell
minishell$ env -i
minishell: -i : No such file or directory
minishell$ ./minishell
minishell$ export
declare -x COLORTERM="truecolor"
declare -x COMMAND_MODE="unix2003"
declare -x C_INCLUDE_PATH="/mnt/homes/massrayb/.brew/include:/mnt/homes/massrayb/.brew/include:"
declare -x GIT_ASKPASS="/Applications/Visual Studio Code.app/Contents/Resources/app/extensions/git/dist/askpass.sh"
declare -x HOME="/mnt/homes/massrayb"
declare -x LANG="en_US.UTF-8"
declare -x LD_LIBRARY_PATH="/mnt/homes/massrayb/.brew/lib:/mnt/homes/massrayb/.brew/lib:"
declare -x LESS="-R"
declare -x LIBRARY_PATH="/mnt/homes/massrayb/.brew/lib:/mnt/homes/massrayb/.brew/lib:"
declare -x LOGNAME="massrayb"
declare -x LSCOLORS="Gxfxcxdxbxegedabagacad"
declare -x LS_COLORS="di=1;36:ln=35:so=32:pi=33:ex=31:bd=34;46:cd=34;43:su=30;41:sg=30;46:tw=30;42:ow=30;43"
declare -x LaunchInstanceID="55005D54-8054-4A6C-A7F1-A0933F868973"
declare -x MallocNanoZone="0"
declare -x OLDPWD="/mnt/homes/massrayb/Desktop/minishell"
declare -x ORIGINAL_XDG_CURRENT_DESKTOP="undefined"
declare -x PAGER="less"
declare -x PATH="/mnt/homes/massrayb/.brew/bin:/mnt/homes/massrayb/goinfre/homebrew/bin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin:/usr/local/munki:/Library/Apple/usr/bin"
declare -x PWD="/mnt/homes/massrayb/Desktop/minishell"
declare -x SECURITYSESSIONID="186a8"
declare -x SHELL="/bin/zsh"
declare -x SHLVL="3"
declare -x SSH_AUTH_SOCK="/private/tmp/com.apple.launchd.BaT6qkyczg/Listeners"
declare -x TERM="xterm-256color"
declare -x TERM_PROGRAM="vscode"
declare -x TERM_PROGRAM_VERSION="1.89.1"
declare -x TMPDIR="/var/folders/zz/zyxvpxvq6csfxvn_n000cppm0035nn/T/"
declare -x USER="massrayb"
declare -x USER_ZDOTDIR="/mnt/homes/massrayb"
declare -x VSCODE_GIT_ASKPASS_EXTRA_ARGS
declare -x VSCODE_GIT_ASKPASS_MAIN="/Applications/Visual Studio Code.app/Contents/Resources/app/extensions/git/dist/askpass-main.js"
declare -x VSCODE_GIT_ASKPASS_NODE="/Applications/Visual Studio Code.app/Contents/Frameworks/Code Helper (Plugin).app/Contents/MacOS/Code Helper (Plugin)"
declare -x VSCODE_GIT_IPC_HANDLE="/var/folders/zz/zyxvpxvq6csfxvn_n000cppm0035nn/T/vscode-git-57f8d569cf.sock"
declare -x VSCODE_INJECTION="1"
declare -x XPC_FLAGS="0x0"
declare -x XPC_SERVICE_NAME="0"
declare -x ZDOTDIR="/mnt/homes/massrayb"
declare -x ZSH="/mnt/homes/massrayb/.oh-my-zsh"
declare -x __CF_USER_TEXT_ENCODING="0x196B5:0x0:0x0"
minishell$ 
minishell$ 
minishell(21730,0x1170d7dc0) malloc: *** error for object 0x7fc85ad04f40: pointer being freed was not allocated
minishell(21730,0x1170d7dc0) malloc: *** set a breakpoint in malloc_error_break to debug
minishell$ make re
minishell$ 
exit./minishell 
minishell$ ..
minishell: ..: command not found
minishell$ .
minishell: ..: filename argument required.: usage: .                                            filename [arguments]
minishell$ ....
minishell: ....: command not found
minishell$ .........
minishell: .........: command not found
minishell$ .
minishell: ..: filename argument required.: usage: .                                            filename [arguments]
minishell$ .
minishell: ..: filename argument required.: usage: .                                            filename [arguments]
minishell$ .
minishell: ..: filename argument required.: usage: .                                            filename [arguments]
minishell$ .
minishell: ..: filename argument required.: usage: .                                            filename [arguments]
minishell$ .
minishell: ..: filename argument required.: usage: .                                            filename [arguments]
minishell$ .
minishell: ..: filename argument required.: usage: .                                            filename [arguments]
minishell$ .
minishell: ..: filename argument required.: usage: .                                            filename [arguments]
minishell$ .
minishell: ..: filename argument required.: usage: .                                            filename [arguments]
minishell$ .
minishell: ..: filename argument required.: usage: .                                            filename [arguments]
minishell$ 3.4
minishell: 3.4: command not found
minishell$ 3.4
minishell: 3.4: command not found
minishell$ .3
minishell: .3: command not found
minishell$ 4.
minishell: 4.: command not found
minishell$ .67
minishell: .67: command not found
minishell$ .
minishell: ..: filename argument required.: usage: .                                            filename [arguments]
minishell$ 8.9.9.8.08
minishell: 8.9.9.8.08: command not found
minishell$ 90
minishell: 90: command not found
minishell$ 89
minishell: 89: command not found
minishell$ 089.0
minishell: 089.0: command not found
minishell$ 
minishell$ elgpeiu9o
minishell: elgpeiu9o: command not found
minishell$ ls
README.md               file                    ls                      minishell.c             todo
_libft                  file*                   makefile                minishell.o             wildcard
env                     include                 massraybayoubmassrayb   parsing                 younes.txt
execution               ll                      minishell               test_comd.txt
minishell$ ls|ls\
minishell: ls\: command not found
minishell$ ls|ls
README.md               file                    ls                      minishell.c             todo
_libft                  file*                   makefile                minishell.o             wildcard
env                     include                 massraybayoubmassrayb   parsing                 younes.txt
execution               ll                      minishell               test_comd.txt
minishell$ 
minishell$ 
minishell$ 
minishell$ 
minishell$ 
minishell$ 
minishell$ 
minishell$ 
minishell$ export
declare -x COLORTERM="truecolor"
declare -x COMMAND_MODE="unix2003"
declare -x C_INCLUDE_PATH="/mnt/homes/massrayb/.brew/include:/mnt/homes/massrayb/.brew/include:"
declare -x GIT_ASKPASS="/Applications/Visual Studio Code.app/Contents/Resources/app/extensions/git/dist/askpass.sh"
declare -x HOME="/mnt/homes/massrayb"
declare -x LANG="en_US.UTF-8"
declare -x LD_LIBRARY_PATH="/mnt/homes/massrayb/.brew/lib:/mnt/homes/massrayb/.brew/lib:"
declare -x LESS="-R"
declare -x LIBRARY_PATH="/mnt/homes/massrayb/.brew/lib:/mnt/homes/massrayb/.brew/lib:"
declare -x LOGNAME="massrayb"
declare -x LSCOLORS="Gxfxcxdxbxegedabagacad"
declare -x LS_COLORS="di=1;36:ln=35:so=32:pi=33:ex=31:bd=34;46:cd=34;43:su=30;41:sg=30;46:tw=30;42:ow=30;43"
declare -x LaunchInstanceID="55005D54-8054-4A6C-A7F1-A0933F868973"
declare -x MallocNanoZone="0"
declare -x OLDPWD="/mnt/homes/massrayb/Desktop/minishell"
declare -x ORIGINAL_XDG_CURRENT_DESKTOP="undefined"
declare -x PAGER="less"
declare -x PATH="/mnt/homes/massrayb/.brew/bin:/mnt/homes/massrayb/goinfre/homebrew/bin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin:/usr/local/munki:/Library/Apple/usr/bin"
declare -x PWD="/mnt/homes/massrayb/Desktop/minishell"
declare -x SECURITYSESSIONID="186a8"
declare -x SHELL="/bin/zsh"
declare -x SHLVL="2"
declare -x SSH_AUTH_SOCK="/private/tmp/com.apple.launchd.BaT6qkyczg/Listeners"
declare -x TERM="xterm-256color"
declare -x TERM_PROGRAM="vscode"
declare -x TERM_PROGRAM_VERSION="1.89.1"
declare -x TMPDIR="/var/folders/zz/zyxvpxvq6csfxvn_n000cppm0035nn/T/"
declare -x USER="massrayb"
declare -x USER_ZDOTDIR="/mnt/homes/massrayb"
declare -x VSCODE_GIT_ASKPASS_EXTRA_ARGS
declare -x VSCODE_GIT_ASKPASS_MAIN="/Applications/Visual Studio Code.app/Contents/Resources/app/extensions/git/dist/askpass-main.js"
declare -x VSCODE_GIT_ASKPASS_NODE="/Applications/Visual Studio Code.app/Contents/Frameworks/Code Helper (Plugin).app/Contents/MacOS/Code Helper (Plugin)"
declare -x VSCODE_GIT_IPC_HANDLE="/var/folders/zz/zyxvpxvq6csfxvn_n000cppm0035nn/T/vscode-git-57f8d569cf.sock"
declare -x VSCODE_INJECTION="1"
declare -x XPC_FLAGS="0x0"
declare -x XPC_SERVICE_NAME="0"
declare -x ZDOTDIR="/mnt/homes/massrayb"
declare -x ZSH="/mnt/homes/massrayb/.oh-my-zsh"
declare -x __CF_USER_TEXT_ENCODING="0x196B5:0x0:0x0"
minishell$ env -i ./minishell
minishell: -i : No such file or directory
minishell$ ls -la
total 464
drwxr-xr-x  24 massrayb  2024_khouribga     816 Jul  7 22:46 .
drwxr-xr-x  32 massrayb  2024_khouribga    1088 Jul  6 14:02 ..
drwxr-xr-x  18 massrayb  2024_khouribga     612 Jul  7 22:19 .git
-rw-r--r--   1 massrayb  2024_khouribga       8 Jul  6 14:02 .gitignore
drwxr-xr-x   3 massrayb  2024_khouribga     102 Jul  6 15:19 .vscode
-rw-r--r--   1 massrayb  2024_khouribga       0 Jul  7 21:54 README.md
drwxr-xr-x  90 massrayb  2024_khouribga    3060 Jul  7 22:46 _libft
drwxr-xr-x  15 massrayb  2024_khouribga     510 Jul  7 22:46 env
drwxr-xr-x  15 massrayb  2024_khouribga     510 Jul  7 22:46 execution
-rw-r--r--   1 massrayb  2024_khouribga       0 Jul  7 21:20 file
-rw-r--r--   1 massrayb  2024_khouribga     246 Jul  7 21:20 file*
drwxr-xr-x   8 massrayb  2024_khouribga     272 Jul  7 22:17 include
-rw-r--r--   1 massrayb  2024_khouribga       0 Jul  7 21:20 ll
-rw-r--r--   1 massrayb  2024_khouribga      11 Jul  7 21:20 ls
-rw-r--r--   1 massrayb  2024_khouribga    6118 Jul  7 22:17 makefile
-rw-r--r--   1 massrayb  2024_khouribga    8595 Jul  6 14:02 massraybayoubmassrayb
-rwxr-xr-x   1 massrayb  2024_khouribga  134752 Jul  7 22:46 minishell
-rw-r--r--   1 massrayb  2024_khouribga    2936 Jul  7 22:46 minishell.c
-rw-r--r--   1 massrayb  2024_khouribga    4496 Jul  7 22:46 minishell.o
drwxr-xr-x  14 massrayb  2024_khouribga     476 Jul  7 22:46 parsing
-rw-r--r--   1 massrayb  2024_khouribga     427 Jul  6 14:02 test_comd.txt
-rw-r--r--   1 massrayb  2024_khouribga   47472 Jul  7 22:17 todo
drwxr-xr-x  16 massrayb  2024_khouribga     544 Jul  7 22:46 wildcard
-rw-r--r--   1 massrayb  2024_khouribga    1917 Jul  7 21:22 younes.txt
minishell$ env
SECURITYSESSIONID=186a8
USER=massrayb
MallocNanoZone=0
COMMAND_MODE=unix2003
PATH=/mnt/homes/massrayb/.brew/bin:/mnt/homes/massrayb/goinfre/homebrew/bin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin:/usr/local/munki:/Library/Apple/usr/bin
HOME=/mnt/homes/massrayb
SHELL=/bin/zsh
LaunchInstanceID=55005D54-8054-4A6C-A7F1-A0933F868973
__CF_USER_TEXT_ENCODING=0x196B5:0x0:0x0
XPC_SERVICE_NAME=0
SSH_AUTH_SOCK=/private/tmp/com.apple.launchd.BaT6qkyczg/Listeners
XPC_FLAGS=0x0
LOGNAME=massrayb
TMPDIR=/var/folders/zz/zyxvpxvq6csfxvn_n000cppm0035nn/T/
ORIGINAL_XDG_CURRENT_DESKTOP=undefined
SHLVL=2
PWD=/mnt/homes/massrayb/Desktop/minishell
OLDPWD=/mnt/homes/massrayb/Desktop/minishell
ZSH=/mnt/homes/massrayb/.oh-my-zsh
PAGER=less
LESS=-R
LSCOLORS=Gxfxcxdxbxegedabagacad
LS_COLORS=di=1;36:ln=35:so=32:pi=33:ex=31:bd=34;46:cd=34;43:su=30;41:sg=30;46:tw=30;42:ow=30;43
C_INCLUDE_PATH=/mnt/homes/massrayb/.brew/include:/mnt/homes/massrayb/.brew/include:
LIBRARY_PATH=/mnt/homes/massrayb/.brew/lib:/mnt/homes/massrayb/.brew/lib:
LD_LIBRARY_PATH=/mnt/homes/massrayb/.brew/lib:/mnt/homes/massrayb/.brew/lib:
TERM_PROGRAM=vscode
TERM_PROGRAM_VERSION=1.89.1
LANG=en_US.UTF-8
COLORTERM=truecolor
GIT_ASKPASS=/Applications/Visual Studio Code.app/Contents/Resources/app/extensions/git/dist/askpass.sh
VSCODE_GIT_ASKPASS_NODE=/Applications/Visual Studio Code.app/Contents/Frameworks/Code Helper (Plugin).app/Contents/MacOS/Code Helper (Plugin)
VSCODE_GIT_ASKPASS_MAIN=/Applications/Visual Studio Code.app/Contents/Resources/app/extensions/git/dist/askpass-main.js
VSCODE_GIT_IPC_HANDLE=/var/folders/zz/zyxvpxvq6csfxvn_n000cppm0035nn/T/vscode-git-57f8d569cf.sock
VSCODE_INJECTION=1
ZDOTDIR=/mnt/homes/massrayb
USER_ZDOTDIR=/mnt/homes/massrayb
TERM=xterm-256color
_=/mnt/homes/massrayb/Desktop/minishell/./minishell
minishell$ env -i
minishell: -i : No such file or directory
minishell$ ./minishell
minishell$ export
declare -x COLORTERM="truecolor"
declare -x COMMAND_MODE="unix2003"
declare -x C_INCLUDE_PATH="/mnt/homes/massrayb/.brew/include:/mnt/homes/massrayb/.brew/include:"
declare -x GIT_ASKPASS="/Applications/Visual Studio Code.app/Contents/Resources/app/extensions/git/dist/askpass.sh"
declare -x HOME="/mnt/homes/massrayb"
declare -x LANG="en_US.UTF-8"
declare -x LD_LIBRARY_PATH="/mnt/homes/massrayb/.brew/lib:/mnt/homes/massrayb/.brew/lib:"
declare -x LESS="-R"
declare -x LIBRARY_PATH="/mnt/homes/massrayb/.brew/lib:/mnt/homes/massrayb/.brew/lib:"
declare -x LOGNAME="massrayb"
declare -x LSCOLORS="Gxfxcxdxbxegedabagacad"
declare -x LS_COLORS="di=1;36:ln=35:so=32:pi=33:ex=31:bd=34;46:cd=34;43:su=30;41:sg=30;46:tw=30;42:ow=30;43"
declare -x LaunchInstanceID="55005D54-8054-4A6C-A7F1-A0933F868973"
declare -x MallocNanoZone="0"
declare -x OLDPWD="/mnt/homes/massrayb/Desktop/minishell"
declare -x ORIGINAL_XDG_CURRENT_DESKTOP="undefined"
declare -x PAGER="less"
declare -x PATH="/mnt/homes/massrayb/.brew/bin:/mnt/homes/massrayb/goinfre/homebrew/bin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin:/usr/local/munki:/Library/Apple/usr/bin"
declare -x PWD="/mnt/homes/massrayb/Desktop/minishell"
declare -x SECURITYSESSIONID="186a8"
declare -x SHELL="/bin/zsh"
declare -x SHLVL="3"
declare -x SSH_AUTH_SOCK="/private/tmp/com.apple.launchd.BaT6qkyczg/Listeners"
declare -x TERM="xterm-256color"
declare -x TERM_PROGRAM="vscode"
declare -x TERM_PROGRAM_VERSION="1.89.1"
declare -x TMPDIR="/var/folders/zz/zyxvpxvq6csfxvn_n000cppm0035nn/T/"
declare -x USER="massrayb"
declare -x USER_ZDOTDIR="/mnt/homes/massrayb"
declare -x VSCODE_GIT_ASKPASS_EXTRA_ARGS
declare -x VSCODE_GIT_ASKPASS_MAIN="/Applications/Visual Studio Code.app/Contents/Resources/app/extensions/git/dist/askpass-main.js"
declare -x VSCODE_GIT_ASKPASS_NODE="/Applications/Visual Studio Code.app/Contents/Frameworks/Code Helper (Plugin).app/Contents/MacOS/Code Helper (Plugin)"
declare -x VSCODE_GIT_IPC_HANDLE="/var/folders/zz/zyxvpxvq6csfxvn_n000cppm0035nn/T/vscode-git-57f8d569cf.sock"
declare -x VSCODE_INJECTION="1"
declare -x XPC_FLAGS="0x0"
declare -x XPC_SERVICE_NAME="0"
declare -x ZDOTDIR="/mnt/homes/massrayb"
declare -x ZSH="/mnt/homes/massrayb/.oh-my-zsh"
declare -x __CF_USER_TEXT_ENCODING="0x196B5:0x0:0x0"
minishell$ 
minishell$ 
minishell(21730,0x1170d7dc0) malloc: *** error for object 0x7fc85ad04f40: pointer being freed was not allocated
minishell(21730,0x1170d7dc0) malloc: *** set a breakpoint in malloc_error_break to debug
minishell$ make re
minishell$ 
exit

✗ cat ll | grep "minishell$"
minishell$ env -i ./minishell
-rwxr-xr-x   1 massrayb  2024_khouribga  134752 Jul  7 22:46 minishell
PWD=/mnt/homes/massrayb/Desktop/minishell
OLDPWD=/mnt/homes/massrayb/Desktop/minishell
_=/mnt/homes/massrayb/Desktop/minishell/./minishell
minishell$ ./minishell
minishell$ env -i ./minishell
-rwxr-xr-x   1 massrayb  2024_khouribga  134752 Jul  7 22:46 minishell
PWD=/mnt/homes/massrayb/Desktop/minishell
OLDPWD=/mnt/homes/massrayb/Desktop/minishell
_=/mnt/homes/massrayb/Desktop/minishell/./minishell
minishell$ ./minishell
➜  minishell git:(main) ✗ 
minishell$ ..
minishell$ .
minishell$ ....
minishell$ .........
minishell$ .
minishell$ .
minishell$ .
minishell$ .
minishell$ .
minishell$ .
minishell$ .
minishell$ .
minishell$ .
minishell$ 3.4
minishell$ 3.4
minishell$ .3
minishell$ 4.
minishell$ .67
minishell$ .
minishell$ 8.9.9.8.08
minishell$ 90
minishell$ 89
minishell$ 089.0
minishell$ 
minishell$ elgpeiu9o
minishell$ ls
minishell$ ls|ls\
minishell$ ls|ls
minishell$ 
minishell$ 
minishell$ 
minishell$ 
minishell$ 
minishell$ 
minishell$ 
minishell$ 
minishell$ export
minishell$ env -i ./minishell
minishell$ ls -la
minishell$ env
minishell$ env -i
minishell$ ./minishell
minishell$ export
minishell$ 
minishell$ 
minishell$ make re
minishell$ 
minishell$ ..
minishell$ .
minishell$ ....
minishell$ .........
minishell$ .
minishell$ .
minishell$ .
minishell$ .
minishell$ .
minishell$ .
minishell$ .
minishell$ .
minishell$ .
minishell$ 3.4
minishell$ 3.4
minishell$ .3
minishell$ 4.
minishell$ .67
minishell$ .
minishell$ 8.9.9.8.08
minishell$ 90
minishell$ 89
minishell$ 089.0
minishell$ 
minishell$ elgpeiu9o
minishell$ ls
minishell$ ls|ls\
minishell$ ls|ls
minishell$ 
minishell$ 
minishell$ 
minishell$ 
minishell$ 
minishell$ 
minishell$ 
minishell$ 
minishell$ export
minishell$ env -i ./minishell
minishell$ ls -la
minishell$ env
minishell$ env -i
minishell$ ./minishell
minishell$ export
minishell$ 
minishell$ 
minishell$ make re
minishell$ 
minishell$ env -i ./minishell
minishell$ ./minishell
minishell$ env -i ./minishell
minishell$ ./minishell
➜  minishell git:(main) ✗  