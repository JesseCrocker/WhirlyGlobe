git submodule init
git submodule update
git submodule sync
rm -rf resources
rm -rf third-party/eigen
rm -rf third-party/clipper
git submodule update
cd resources
git checkout master
cd ../
cd third-party/eigen
git checkout master
cd ../../
cd third-party/clipper
git checkout master
cd ../..
git submodule update
