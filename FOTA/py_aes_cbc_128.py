from base64 import b64encode, b64decode
from binascii import unhexlify
from Crypto.Cipher import AES
from Crypto.Util.Padding import pad, unpad


class AES_128_CBC:  
    def __init__(self):
        self.iv = "000102030405060708090a0b0c0d0e0f" #use the same key as used in code
        self.password = "2b7e151628aed2a6abf7158809cf4f3c" #use the same key as used in code
        print(f"IV: {self.iv}")
        print(f"PWD: {self.password}")
# Convert Hex String to Binary
        self.iv = unhexlify(self.iv)
        self.password = unhexlify(self.password)
    
    def encrypt(self,plaintmsg):
        # Encipher Text
        self.cipher = AES.new(self.password, AES.MODE_CBC, self.iv)
        self.cipher_text = self.cipher.encrypt(plaintmsg)
        return self.cipher_text

    def decrypt(self,encmsg):
        # Decipher cipher text
        decipher = AES.new(self.password, AES.MODE_CBC, self.iv)
        # UnPad Based on AES Block Size
        plaintext = decipher.decrypt(encmsg)
        print(plaintext)
        return encmsg