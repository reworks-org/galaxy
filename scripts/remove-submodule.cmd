git submodule deinit -f ../dependencies/%1
rm -rf ../.git/modules/dependencies/%1
git rm -f ../dependencies/%1