
mkdir -p jhgs

joinhg -a -2.5 -b 181.5 -h 1 -o jhgs/wdae  ot_*/wdae.hg
joinhg -a -2.5 -b 181.5 -h 1 -o jhgs/wdae2 ot_*/wdae2.hg
joinhg -a -2.5 -b 181.5 -h 1 -o jhgs/wdag  ot_*/wdag.hg
joinhg -a -2.5 -b 181.5 -h 1 -o jhgs/wdag2 ot_*/wdag2.hg

joinhg -a 0 -b 1e5 -h 1e3 -o jhgs/te0   ot_*/te0.hg
joinhg -a 0 -b 1e5 -h 1e3 -o jhgs/te15  ot_*/te15.hg
joinhg -a 0 -b 1e5 -h 1e3 -o jhgs/te30  ot_*/te30.hg
joinhg -a 0 -b 1e5 -h 1e3 -o jhgs/te45  ot_*/te45.hg
joinhg -a 0 -b 1e5 -h 1e3 -o jhgs/te60  ot_*/te60.hg
joinhg -a 0 -b 1e5 -h 1e3 -o jhgs/te75  ot_*/te75.hg
joinhg -a 0 -b 1e5 -h 1e3 -o jhgs/te90  ot_*/te90.hg
joinhg -a 0 -b 1e5 -h 1e3 -o jhgs/te105 ot_*/te105.hg
joinhg -a 0 -b 1e5 -h 1e3 -o jhgs/te120 ot_*/te120.hg
joinhg -a 0 -b 1e5 -h 1e3 -o jhgs/te135 ot_*/te135.hg
joinhg -a 0 -b 1e5 -h 1e3 -o jhgs/te150 ot_*/te150.hg
joinhg -a 0 -b 1e5 -h 1e3 -o jhgs/te165 ot_*/te165.hg
joinhg -a 0 -b 1e5 -h 1e3 -o jhgs/te180 ot_*/te180.hg

joinhg -a 0 -b 1e5 -h 1e3 -o jhgs/tg180 ot_*/tg180.hg


joinhg -a 0 -b 1e5 -h 1e3 -o jhgs/re0   or_*/te0.hg
joinhg -a 0 -b 1e5 -h 1e3 -o jhgs/rwe0  orw_*/te0.hg
joinhg -a 0 -b 1e5 -h 1e3 -o jhgs/rw2e0 orw2_*/te0.hg
joinhg -a 0 -b 1e5 -h 1e3 -o jhgs/rw3e0 orw3_*/te0.hg

joinhg -a 0 -b 1e5 -h 1e3 -o jhgs/re180   or_*/te180.hg
joinhg -a 0 -b 1e5 -h 1e3 -o jhgs/rwe180  orw_*/te180.hg
joinhg -a 0 -b 1e5 -h 1e3 -o jhgs/rw2e180 orw2_*/te180.hg
joinhg -a 0 -b 1e5 -h 1e3 -o jhgs/rw3e180 orw3_*/te180.hg

joinhg -a 0 -b 1e5 -h 1e3 -o jhgs/rg180   or_*/tg180.hg
joinhg -a 0 -b 1e5 -h 1e3 -o jhgs/rwg180  orw_*/tg180.hg
joinhg -a 0 -b 1e5 -h 1e3 -o jhgs/rw2g180 orw2_*/tg180.hg
joinhg -a 0 -b 1e5 -h 1e3 -o jhgs/rw3g180 orw3_*/tg180.hg

