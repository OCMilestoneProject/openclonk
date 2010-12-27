#!/bin/sh
LIBS_TO_BUNDLE="/opt/local/lib/lib(crypto|z\.|iconv|jpeg|png|GLEW|llvm|SDL|SDL_mixer|freetype)[^ ]+"

cd $TARGET_BUILD_DIR
pwd
for lib in `otool -L $EXECUTABLE_PATH | grep -Eo "$LIBS_TO_BUNDLE"`; do
  base=`basename $lib`
  mkdir -p $FRAMEWORKS_FOLDER_PATH
  bundle_path=$FRAMEWORKS_FOLDER_PATH/$base
  id=@executable_path/../Frameworks/$base
  
  echo Bundling $base... cp $lib $bundle_path
  cp $lib $bundle_path
  chmod u+w $bundle_path
  install_name_tool -id $id $bundle_path
  install_name_tool -change $lib $id $EXECUTABLE_PATH
done
