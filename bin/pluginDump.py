#!/usr/bin/env python             
from collections import defaultdict
# pluginDump.py
import os
import sys

plugins = defaultdict(list)
################################################################
def GetPlugins(dirName) :
  if verbose >= 2: print "GETPLUGINS %s : %i "%(dirName,os.path.exists("src/plugin.cc"))
  if os.path.exists(dirName+"/src/plugin.cc") :
   fin = open(dirName+"/src/plugin.cc","r")

   lines = fin.readlines()
   for line in lines :
     if verbose >= 2 : print "LOOP plugin.cc  %s : %s"%(line,line.find("PLUGINSVC_FACTORY"))
     if line.find("PLUGINSVC_FACTORY") != -1 : 
       if line.find("//") == -1 : 
         ifp = line.find("(")
         ifc = line.find(",")
         ifa = line.find("*")
         className = line[ifp+1:ifc]
         parentName = line[ifc+1:ifa]
         if verbose >= 2 : print " PLUGIN FOUND %s : %s, %s "%(line,className,parentName)
         plugins[parentName.lstrip()].append(className.lstrip())

################################################################
def findDirs(dirName) :
  if verbose >= 2 : print " FIND DIRECTORY %s " % dirName
  dirs = os.listdir(dirName)
  bIsEndDir = 0
  for newDir in dirs : 
#    if verbose >= 2 : print " LOOP FILE %s " % newDir
    if (os.path.isdir(dirName+"/"+newDir)) and (newDir != "."):
      if verbose >= 2 : print " LOOP DIRECTORY %s " % newDir
      if (newDir != "src") and (newDir != "include") :
        newDir = dirName + "/" + newDir
        if verbose >= 2 : print " LOOP DIRECTORY2 %s " % newDir
## exceptions, because only contain executables
#        if (newDir.find("PluginChecker") == -1) and (newDir.find("PluginManager") == -1 ) and (newDir.find("PluginRefresh") == -1 ) and (newDir.find("PluginDumper") == -1 ) and (newDir.find("GamosApplication") == -1):
        findDirs(newDir)
      else :
        bIsEndDir = 1 # it hass src and include directories

## end loop to subdirec, write cmake files
#t  WriteCMakeList(dirName, bIsEndDir)
  if bIsEndDir :
     GetPlugins(dirName)


################################################################
verbose = 0
argc = len(sys.argv)
gamosdir = os.getenv("GAMOSINSTALL")
if argc == 1 : 
  dirName = gamosdir+"/source"
else :
  dirName = sys.argv[1]

findDirs(dirName)

# print plugins
for k,v in plugins.items() :
  for cl in v:
     print ("CLASS %s : %s"%(k,cl))
