
source $HOME/lepts/conflepts.sh

FL="
wrapper 1
material gold
generator point
gparticle gamma
genergy 60*keV
stopenergyelectron 1*keV
halflength 5*um
beamon 10000

xsdat gold
#xsdat goldwater
#halflengthwater 20*um

#vis $LEPTSDIR/XsectDat/visOGLIX.mac
#batch 24:00:00
saveimg 100000000
verbosedetector -2
"

# para dibujar:
joblepts $FL beamon 100 halflength '50*um' gposition '-2*cm 0 0' verbosehistogram 3
exit



# Transmi varios espesores
#WS="10 20 30 50 70 100 150 200 300 400 500 600 700 800 900"
#for W in $WS ; do
#    let w=$W/2
#    #FL2="batch 24:00:00 dirjob ow_$W beamon 10000"
#    FL2="batch 24:00:00 dirjob owcm_$W beamon 100000"
#    joblepts $FL $FL2 halflength ${w}'*um' gposition '-2*cm 0 0'
#done
##grep Data: ow_*/tg0.hg
#exit



# Transmi 100um espectros
FL2="halflength 50*um batch 24:00:00 beamon 500000 "
for((i=1;i<21;i++)) ; do
    let s=i+1000
    FL3="seed $s dirjob ot_$s"
    joblepts $FL $FL2 $FL3 gposition '-2*cm 0 0'
done




# Reflex 100um
FL2="halflength 50*um batch 24:00:00 beamon 125000"
for((i=1;i<81;i++)) ; do
    let s=i+1000
    FL3="seed $s dirjob or_$s"
    #joblepts $FL $FL2 $FL3 gposition '-0.7*cm -1*cm 0' gdirection '0.7 1 0'

    #FL3="seed $s dirjob orw_$s xsdat goldwater halflengthwater 1*um"
    #joblepts $FL $FL2 $FL3 gposition '-0.7*cm -1*cm 0' gdirection '0.7 1 0'
    #FL3="seed $s dirjob orw2_$s xsdat goldwater halflengthwater 5*um"
    #joblepts $FL $FL2 $FL3 gposition '-0.7*cm -1*cm 0' gdirection '0.7 1 0'
    FL3="seed $s dirjob orw3_$s xsdat goldwater halflengthwater 25*um"
    joblepts $FL $FL2 $FL3 gposition '-0.7*cm -1*cm 0' gdirection '0.7 1 0'
done
