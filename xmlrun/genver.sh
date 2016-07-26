GITVER=$("C:\Program Files (x86)\Git\bin\git" describe | sed s/-g.*$// | sed s/-/./ | sed s/^v//)

echo $GITVER