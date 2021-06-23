@ECHO OFF
ECHO Adding files...
git add -A

set /p commitMessage="Write commit message:"

git commit -m commitMessage

ECHO pushing...

git push origin master

ECHO Thanks :)

ECHO Pushing...
PAUSE