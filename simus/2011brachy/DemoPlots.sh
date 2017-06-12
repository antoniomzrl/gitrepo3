source $HOME/lepts/conflepts.sh

set -x


wrl geombrachy.wrl

drawpw sourcetarget.bcd

drawpw sourcetargetslice.bcd
drawpw secondary.bcd

drawpw singletrack.bcd
drawpw hundredtracks.bcd
drawpw thousandtracks.bcd

amide xdimg*hv

drawpw xdimg0001000.bcd

drawpw xdimg0100000.bcd
