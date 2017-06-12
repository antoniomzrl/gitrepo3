  is_curl=`which curl |wc -w`
  is_wget=`which wget |wc -w`

if test $is_wget == 2 ; then
curl -v --connect-timeout 30 -f -L -O http://fismed.ciemat.es/GAMOS/VRMLviewers/vrmlview
curl -v --connect-timeout 30 -f -L -O http://fismed.ciemat.es/GAMOS/VRMLviewers/libstdc++-libc6.1-1.so.2
curl -v --connect-timeout 30 -f -L -O http://fismed.ciemat.es/GAMOS/VRMLviewers/g4_00.wrl
curl -v --connect-timeout 30 -f -L -O http://fismed.ciemat.es/GAMOS/VRMLviewers/g4_01.wrl
 elif test $is_curl == 1 ; then
  wget http://fismed.ciemat.es/GAMOS/VRMLviewers/vrmlview
wget http://fismed.ciemat.es/GAMOS/VRMLviewers/g4_00.wrl
wget http://fismed.ciemat.es/GAMOS/VRMLviewers/g4_01.wrl
wget http://fismed.ciemat.es/GAMOS/VRMLviewers/libstdc++-libc6.1-1.so.2
fi

chmod +x vrmlview
mv vrmlview ${GAMOSDIR}/${GAMOS_VER}/bin/${G4SYSTEM}
mv libstdc++-libc6.1-1.so.2 ${GAMOSDIR}/${GAMOS_VER}/lib/${G4SYSTEM}
rehash
${GAMOSDIR}/${GAMOS_VER}/bin/${G4SYSTEM}/vrmlview g4_01.wrl
