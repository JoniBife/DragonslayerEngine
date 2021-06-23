@ECHO OFF
ECHO =================== Adding files... ===================
git add -A

set /p commitMessage="Write commit message: "
set /p remote="Remote: "
set /p branch="Branch: "

ECHO =================== Committing... =====================

git commit -m commitMessage

ECHO =================== Pushing... ========================

git push remote branch

ECHO =================== Thanks :) =========================
QUIT