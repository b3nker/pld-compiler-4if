[ -d ./pld-test-output ] && rm -r ./pld-test-output
chmod 755 pld-wrapper.sh
python3 pld-test.py tests/$1
