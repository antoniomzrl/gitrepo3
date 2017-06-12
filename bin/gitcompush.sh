# 1st time
# git init
# git add *
# git commit -a -m 'first commit'
# git remote add origin https://github.com/antoniomzrl/gitrepoapp.git
# git push -u origin master

set -x
git status
git add --all .
#commit
git commit -a -v -m "`hostname` `date +%F-%T`"

# local -> server
git push

# server -> local
# git pull --dry-run
# git pull

#recover:
# git log path/to/file or gitk para ver el SHA1
# git checkout

# avoid user/passw
# git config --global credential.helper cache
# git config --global credential.helper "cache --timeout=36000"
