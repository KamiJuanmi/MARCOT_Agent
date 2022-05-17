import subprocess

f = open("./test/test_seg_fault","wb")

for i in range(20):
    try:
        subprocess.call("./cliente", stdout=f, stderr=f)
    except Exception as e:
            print(str(e))