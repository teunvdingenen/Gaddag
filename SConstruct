# -*- Python -*-
# CopyLeft 2011 Teun van Dingenen. No License
# --------------------------------------------------------------------------

import os
env = Environment(ENV = {'PATH' : os.environ['PATH']})

#env.Append(CCFLAGS = ' -g', FOO = ['foo.yyy'])
env.Append( CCFLAGS = '-g -std=c++11' )
Export( "env" )
# if ARGUMENTS.get('debug', 0):
#    else:
#        env = Environment()

gaddag_sources = [ ]
files=os.listdir("src/")
for filename in files:
    if filename.endswith('.cpp'):
        gaddag_sources.append( 'src/'+filename )

env.SharedLibrary( target='bin/gaddag', source = gaddag_sources )

env.Program( target='play_scrabble', source=[ 'bin/libgaddag.so', 'main.cpp' ] )
