cc -o make_cacert make_cacert.c
cc -o make_mycert make_mycert.c
cc -o make_mykey make_mykey.c
./make_cacert -n ca_jeys -o ca_jeys.c ca_jeys.der
./make_cacert -n ca_mac -o ca_mac.c ca_mac.der
./make_cacert -n ca_nintendo -o ca_nintendo.c ca_nintendo.der
./make_cacert -n ca_rsa -o ca_rsa.c ca_rsa.der
./make_cacert -n ca_rsaclass3 -o ca_rsaclass3.c ca_rsaclass3.der
./make_mycert -n mycert -o mycert.c cert_namahage.der
./make_mykey -n mykey -o mykey.c key_namahage.der

