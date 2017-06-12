set -x
echo $0
echo $PWD


for d in *_*sh ; do
    sh -x $d &
done
