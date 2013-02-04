import MDAnalysis as mda
from MDAnalysis.analysis.align import *
import sys
import matplotlib.pylab as plt
import numpy as np

def RMSD(psf,dcd):
    U = mda.Universe(psf,dcd)
    Ref = U.atoms.coordinates()
    numframes = U.trajectory.numframes
    RMSD = np.zeros(numframes)

    for i in xrange(0,numframes):
        RMSD[i] = rmsd(U.atoms.coordinates(),Ref)

        if i < numframes - 1:
            U.trajectory.next()

    return RMSD

if __name__ == '__main__':
    psf = sys.argv[1]
    dcd = sys.argv[2]

    if(len(sys.argv) > 3):
        if(sys.argv[3] == 'plot'):
            R = RMSD(psf,dcd)
            plt.plot(R)
            plt.show()
            R = RMSD(psf,dcd)
            np.savetxt('rmsd.dat',R)
        else:
            R = RMSD(psf,dcd)
            np.savetxt('rmsd.dat',R)
