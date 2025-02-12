from py_aes_cbc_128 import *
import tkinter as tk
from tkinter import filedialog
import os

class BinaryFileEncryption:
    def __init__(self):
        print("Binary File Encryption")
        self.padSize = 1024
        self.paddedRawFile = "Output/Padded1024_Non_Enc_Bin.bin"
        self.EncFile = "Output/Padded1024_Enc_Bin.bin"

#todo: open a bianry file    
    def BrowseBinFile(self):
        root = tk.Tk()
        root.withdraw()
        currdir = os.getcwd()
        self.filepath = filedialog.askopenfilename(filetypes=(("BinaryTxt", "*.bin"),("All Files", "*.*")))

#todo: Padd the binary file for 1024 byte alignment with 0xFF
    def PaddBinFile(self):
        self.file_size = os.stat(self.filepath)

        if self.file_size.st_size / self.padSize != 0:
            self.bytesToPad = (int(self.file_size.st_size / self.padSize) + 1)
            self.bytesToPad = self.padSize * self.bytesToPad
            self.bytesToPad = self.bytesToPad - self.file_size.st_size

        self.appendBytes = bytearray(self.bytesToPad)

        for i in range(0,self.bytesToPad):
            self.appendBytes[i] = 0xFF

        with open(self.paddedRawFile,"wb") as self.PaddedFile:
            with open(self.filepath,"rb") as self.orgBinFile:
                self.readRaw = self.orgBinFile.read()
                self.PaddedFile.write(self.readRaw)
        self.orgBinFile.close()
        self.PaddedFile.close()

        with open(self.paddedRawFile,"ab") as self.PaddedFile:
            self.PaddedFile.write(self.appendBytes)
        
        self.PaddedFile.close()
        self.file_size = os.stat(self.paddedRawFile)
        print(self.file_size.st_size)

        

#todo: Enc the binary file.
    def EncBinFile(self):
        self.aes= AES_128_CBC()
        file_size = os.path.getsize(self.paddedRawFile)
        with open (self.EncFile,"wb") as self.encFile:
            with open (self.paddedRawFile,"rb") as self.paddedFile:
                while file_size != 0:
                    self.readraw = self.paddedFile.read(1024)
                    self.encData = self.aes.encrypt(self.readraw)
                    print("RAW DATA %d ",len(self.readraw))
                    print(self.readraw)
                    print("ENC DATA %d" ,len(self.encData))
                    print(self.encData)
                    self.encFile.write(self.encData)
                    file_size = file_size - 1024
        
        self.encFile.close()
        self.paddedFile.close()


#todo: store encrypted data in EncBin File
        

if __name__=="__main__":
    enc = BinaryFileEncryption()
    enc.BrowseBinFile()
    enc.PaddBinFile()
    enc.EncBinFile()