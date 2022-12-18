---
layout: post
readtime: true
tags: [dll-injection, reversing, malware]
comments: true
---

This is the official writeup for the reverse engineering challenge YameteKudasai from Backdoor CTF 2022.

First of all, let's try to understand the working of the program.

## Step 1 -> Generating Hashes and DLL Injection

First of all, the exe generates a random value in the range (100000000, 900000000), computes its SHA1 hash and saves it into the registry under `HKEY_CURRENT_USER\SOFTWARE\Yamete` with the keyname `secret`. After that, it computes `random_value*2 + 10`, generates a numeric string and inserts `random_value*2+10` into the numeric string at index 15 and saves the resulting numeric string into `HKEY_CURRENT_USER\SOFTWARE\Yamete` with the keyname `numpath`. After saving stuff, the program enumerates through the processes running on the system, selects the first process that can be opened with the `PROCESS_ALL_ACCESS` flag, creates a random directory in temp path ,downloads a DLL `https://bincourse.s3.us-west-1.amazonaws.com/kudasai.dll` into that directory. After the dll is downloaded, the program selects a process that can be opened with the `PROCESS_ALL_ACCESS` flag and performs a DLL Injection into the target process (kudasai.dll is injected into the target process).

## Step2 -> Analyzing kudasai.dll

The DLL can be extracted from the temp path using dynamic analysis. Running `strings` on `kudasai.dll`, a url `https://bincourse.s3.us-west-1.amazonaws.com/kudasai.pdb` is revealed. `kudasai.pdb` stores debug information for `kudasai.dll`. Loading both `kudasai.dll` and `kudasai.pdb` into IDA, everything becomes clear. Using IDA and some dynamic analysis, we see that the `EncryptSecret()` is called as soon as the DLL is loaded into a process.

### Custom allocator with std::vector

`EncryptSecret()` function calls `GenerateAESkey()` which stores 32 values in a `std::vector` and returns it. This vector is not an ordinary vector. A custom allocator is used for this vector. For more details, checkout [https://www.gamedeveloper.com/programming/custom-vector-allocation](https://www.gamedeveloper.com/programming/custom-vector-allocation).

Using this allocator, the constructor of the vector was modified such that

```cpp
void construct(pointer ptr, const T &val)
	{
		new (ptr) T((val / 2 * 3) + 4 + ctr);
		ctr++;
	}
```
This constructor clearly modifies the actual value that was pushed into the vector and stores a different value in it according to the expression mentined above. Now that we know how the vector works, we can find out the AES key which comes out to be 
```
[64, 35, 18, 25, 32, 36, 43, 122, 24, 28, 134, 129, 112, 125, 45, 61, 95, 111, 127, 83, 78, 97, 74, 96, 151, 59, 90, 127, 146, 141, 145, 83]
```

## Step 3-> Encrypting the hash using 256 byte AES ECB

After generating the AES key, the DLL opens the registry key `HKEY_CURRENT_USER\SOFTWARE\Yamete`, and finds the value stored in the value `secret`. This is the sha1 hash initially set by the main exe. The dll selects the first 32 characters from this hash, encrypts it using 256 byte AES ECB and stores the ciphertext in the registry used before, under the name `yametekudasai`. 

## Step 4-> Back to YameteKudasai.exe
After that control goes back to the provided exe file. It opens `flag.txt`, reads data from it and encrypts it using RC4 encryption algorithm. The key for RC4 encryption is retrieved from the registry (`yametekudasai`), which was previously set due to dll injection. The exe saves the encrypted data to a file whose initial name is retrieved from the registry (`numpath`), it's extension is set to b4ckd00r and then the file is written to the folder from where the exe was run. 


## Solution
The hash can be retrieved from the name of the encrypted file. Remember that the value stored under `numpath` in `HKEY_CURRENT_USER\SOFTWARE\Yamete` is the filename (excluding the file extension). Recall that the program generates a numeric string and inserts `random_value*2+10` into the randomly generated numeric string at index 15. This can be used to calculate `random_value` which can be used to calculate the hash and eventually you can get the RC4 key following the steps mentioned above. The complete solve script is:

```py
from hashlib import sha1
from Crypto.Cipher import AES,ARC4
filename = "453047533093372200013162620385759576391"
aes_key = [64, 35, 18, 25, 32, 36, 43, 122, 24, 28, 134, 129, 112, 125, 45, 61, 95, 111, 127, 83, 78, 97, 74, 96, 151, 59, 90, 127, 146, 141, 145, 83]

with open("453047533093372200013162620385759576391.b4ckd00r","rb") as f:
    data = f.read()
    rand_val = (int(filename[15:-15])-10)//2
    hash = sha1(str(rand_val).encode()).hexdigest()[:32]
    aes_cipher = AES.new(bytearray(aes_key),AES.MODE_ECB)
    rc4_key = aes_cipher.encrypt(hash.encode())
    rc4_cipher = ARC4.new(rc4_key)
    print(rc4_cipher.decrypt(data))
```
<img src='https://github.com/0xSh4dy/0xSh4dy.github.io/blob/master/assets/img/Windows%20PowerShell%2018-12-2022%2015_14_17.png?raw=true'>