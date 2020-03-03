#!/usr/bin/python3

import os,sys

def createArchive():
	os.system("tar -cf "+sys.argv[2].split("/")[-1]+"_compress.tar -T /dev/null")

def extractArchive(fichier):
	os.system("mkdir -p uncompressed && tar -xf "+fichier.split("/")[-1]+".tar.gz -C uncompressed")


def check(repertoire):
	filename = os.path.realpath(__file__)
	for r,d,f in os.walk(repertoire):
		for each in f:
			if each==sys.argv[0].split("./")[1]:
				print("Script is in requested directory. Cannot compress")
				quit()

def create_copy(repertoire):
	os.system("mkdir "+repertoire.split("/")[-1]+"_compressed && cp -r --parents "+repertoire+" "+repertoire.split("/")[-1]+"_compressed/")
	for root,dirs,files in os.walk(os.path.realpath(sys.argv[0]).rsplit("/",1)[0]+"/"+repertoire.split("/")[-1]+"_compressed"):
		for each in files:
			if not each.endswith("hman"):
				os.remove(os.path.join(root,each))

	for r,d,f in os.walk(repertoire):
		for each in f:
			if each.endswith("hman"):
				os.remove(os.path.join(r,each))


def compress(repertoire):

	forbidden_ext=['pdf','o','ppt','zip','dot','c','cpp','py','gz']
	lst = os.listdir(repertoire)
	for each in lst:
		if os.path.isdir(repertoire+'/'+each):
			compress(repertoire+'/'+each)
		elif os.path.isfile(repertoire+'/'+each):
			#On peut chercher les fichier.endswith(".txt") mais pas toutes les fichiers du type text
			#finissent par .txt .
			if not each.split(".")[-1] in forbidden_ext:
				try:
					os.system("./huffman -c "+os.path.realpath(repertoire)+'/'+each)
				except:
					print("couldnt compress this file: "+os.path.realpath(repertoire)+'/'+each)



def archive(repertoire):
	os.system("tar uf "+repertoire.split('/')[-1]+"_compress.tar "+repertoire.split("/")[-1]+"_compressed ")
	os.system("gzip "+repertoire.split('/')[-1]+"_compress.tar")
	os.system("rm -rf "+repertoire.split("/")[-1]+"_compressed")

def decompress(repertoire):
	lst=[]
	for root,dirs,files in os.walk(repertoire):
		for each in files:
			if each.endswith(".hman"):
				lst.append(os.path.join(root,each))

	for every in lst:
		os.system("./huffman -d "+every)
		os.system("rm "+every)
			

if __name__=="__main__":
	if(len(sys.argv)!=3):
		print("Usage: ./script.py [-c/-d] repertoire")
		sys.exit()
	else:
		if(sys.argv[1]=="-c"):
			check(sys.argv[2]);
			createArchive()
			compress(sys.argv[2])
			create_copy(sys.argv[2])
			archive(sys.argv[2])
			print("\nsuccesfully compressed folder")
		elif(sys.argv[1]=="-d"):
			extractArchive(sys.argv[2])
			decompress("uncompressed")
			print("Succesfully decompressed")