#
#This program generates random enigma settings and saves them, then encrypts known plaintext with those settings and saves them to enable the c++ code to be tested against a known good machine
#

from random import randint, seed
from enigma import Machine
from enigma.builtin_rotors import Rotors
import time

def numberToLetter(n):
    return chr(n + 65)

#Makes a random settings string with inputed options
#Rotor options is list of ints [1,8]
#Extra options is list of chars ['T'/'G']
#Reflector options is list of chars ['A','B','C','b','c']
#Number of plugs is int
def makeSettingString(rotorOptions, reflectorOptions, extraOptions, numberOfPlugs):
    #String to hold random settings
    randomString = ""

    #Choose 3 rotors and their initial settings, making sure to never choose one rotor twice
    rotorAlreadyChosen = [False]*len(rotorOptions)
    for i in range(3):
        while (True):
            rotorIndex = randint(0,(len(rotorOptions) - 1))
            if (not rotorAlreadyChosen[rotorIndex]):
                rotorAlreadyChosen[rotorIndex] = True
                break
        randomString += str(rotorOptions[rotorIndex]) + " "
        randomString += str(randint(0,25)) + " "
        randomString += str(randint(0,25)) + " "

    #Choose combination of reflector and fourth rotor (and its initial settings)
    randomString += reflectorOptions[randint(0,(len(reflectorOptions)-1))] + " "
    randomString += extraOptions[randint(0,(len(extraOptions) - 1))] + " "
    randomString += str(randint(0,25)) + " "
    randomString += str(randint(0,25))

    #Generate random plugboard pairs
    plugAlreadyChose = [False]*26
    for i in range(numberOfPlugs):
        while (True):
            plugOne = randint(0,25)
            if (not plugAlreadyChose[plugOne]): 
                plugAlreadyChose[plugOne] = True
                break
        while (True):
            plugTwo = randint(0,25)
            if (not plugAlreadyChose[plugTwo]): 
                plugAlreadyChose[plugTwo] = True
                break
        randomString += " " + str(plugOne) + " " + str(plugTwo)

    #Terminate string and return it
    randomString += "\n"
    return randomString


#Reads a settingString and returns a machine set up accordingly
def readSettingString(settingString):
    #Split settings into individual numbers and letters
    setting = str.split(settingString)
    rotorName = ["I", "II", "III", "IV", "V", "VI", "VII", "VIII"]

    #Do rotor settings for 4 rotor version
    if (setting[9] == "b" or setting[9] == "c"):
        #change 4th rotor names to python compatible
        if (setting[9] == "b"): setting[9] = "B-thin"        
        if (setting[9] == "c"): setting[9] = "C-thin"
        if (setting[10] == "T"): setting[10] = "Beta"
        if (setting[10] == "G"): setting[10] = "Gamma"
        
        #Build machine
        machine = Machine([Rotors[setting[10]](), Rotors[rotorName[int(setting[0])-1]](), Rotors[rotorName[int(setting[3])-1]](), Rotors[rotorName[int(setting[6])-1]]()], Rotors[setting[9]]())

        #Set rotor ring settings
        machine.rotors[0].set_setting(int(setting[12]) + 1)
        machine.rotors[1].set_setting(int(setting[2]) + 1)
        machine.rotors[2].set_setting(int(setting[5]) + 1)
        machine.rotors[3].set_setting(int(setting[8]) + 1)

        #Set rotor initial positions
        machine.rotors[0].set_position(numberToLetter(int(setting[11])))
        machine.rotors[1].set_position(numberToLetter(int(setting[1])))
        machine.rotors[2].set_position(numberToLetter(int(setting[4])))
        machine.rotors[3].set_position(numberToLetter(int(setting[7])))

    #Do rotor settings for 3 rotor version
    else:
        #Build machine
        machine = Machine([Rotors[rotorName[int(setting[0])-1]](), Rotors[rotorName[int(setting[3])-1]](), Rotors[rotorName[int(setting[6])-1]]()], Rotors[setting[9]]())

        #Set rotor ring settings
        machine.rotors[0].set_setting(int(setting[2]) + 1)
        machine.rotors[1].set_setting(int(setting[5]) + 1)
        machine.rotors[2].set_setting(int(setting[8]) + 1)

        #Set rotor initial positions
        machine.rotors[0].set_position(numberToLetter(int(setting[1])))
        machine.rotors[1].set_position(numberToLetter(int(setting[4])))
        machine.rotors[2].set_position(numberToLetter(int(setting[7])))

    #Build plugboard mapping and add to machine
    numberOfPlugs = int((len(setting)-13)/2)
    plugMappings = ""
    plugIndex = 0
    if (numberOfPlugs > 0):
        for x in range(numberOfPlugs):
            plugMappings += numberToLetter(int(setting[13+plugIndex]))
            plugIndex += 1
            plugMappings += numberToLetter(int(setting[13+plugIndex]))
            plugIndex += 1
            plugMappings += " "
        plugMappings = plugMappings[:-1]
        machine.set_plugboard_mappings(plugMappings)
    
    return machine

#Files
path = "SelfTestData/"
settingsFilename = "Settings.txt"
plaintextFilename = "Plaintext.txt"

#Number of settings generated
n = 2000

#Random seed
seed(271828)

#Options
rotorOptions = [1,2,3,4,5,6,7,8]
reflectorOptions = ["B","C","b","c"]
extraOptions = ["T","G"] #T for beta rotor, G for gamma rotor
minNumberOfPlugPairs = 0
maxNumberOfPlugPairs = 13


#Begin code

#Timing
startTime = time.perf_counter()

print("Generating settings and ciphertext")

#Read plaintext
plaintextFile = open(path + plaintextFilename, "r")
plaintext = plaintextFile.read()
plaintextFile.close()

#Open settings file and write plaintext filename
settingsFile = open(path + settingsFilename, "w")
settingsFile.write(plaintextFilename)
settingsFile.write("\n")

#Write data
for i in range(n):
    print("Generating " + str(i))
    settingString = makeSettingString(rotorOptions, reflectorOptions, extraOptions, randint(minNumberOfPlugPairs, maxNumberOfPlugPairs))
    settingsFile.write(settingString)
    ciphertextFilename = str(i) + ".txt"
    ciphertextFile = open(path + ciphertextFilename, "w")
    machine = readSettingString(settingString)
    ciphertextFile.write(machine.encode(plaintext))
    ciphertextFile.close()
settingsFile.close()

print(str(n) + " settings generated successfully")
print("Time taken: " + str(time.perf_counter() - startTime) + "s")
input()