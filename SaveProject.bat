@ECHO OFF

set /p repoLocation="Repository location: "

cd repoLocation

ECHO =================== Adding files... ===================
git add -A

set /p commitMessage="Write commit message: "

ECHO =================== Committing... =====================
git commit -m commitMessage

set /p remote="Remote: "
set /p branch="Branch: "

ECHO =================== Pushing... ========================
git push remote branch

ECHO =================== Thanks :) =========================
PAUSE