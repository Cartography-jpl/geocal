import os

def findepend(pdf, pdfDir):
    print "finding dependencies of " + pdf + " in " + pdfDir + " ..."

    dirList = os.listdir(pdfDir)

    # this will hold all pdfs, including suffix
    pdfList = []

    # this will include all files, excluding pdf suffix, if any
    allList = []

    for entry in dirList:
        if os.path.isdir(pdfDir + "/" + entry):
            print "ignoring directory " + entry
            continue

        if entry[-4:] == ".pdf":
            pdfList.append(entry)
            entry = entry[0:-4]

        if not entry in allList:
            allList.append(entry)

    depends = findepend_0(pdf, pdfDir, pdfList, allList, [])

    binaryCount = 0
    pdfCount = 0

    finalDepends = []
    for entry in depends:
        if os.path.exists(pdfDir + "/" + entry):
            binaryCount += 1
            finalDepends.append(entry)
        if os.path.exists(pdfDir + "/" + entry + ".pdf"):
            pdfCount += 1
            finalDepends.append(entry + ".pdf")

    depends = finalDepends

    print pdf + " depends on " + str(len(depends)) + " files in " + pdfDir + " including " + str(binaryCount) + \
        " binaries and " + str(pdfCount) + " pdf script files"
    print "There are " + str(len(pdfList)) + " pdf files among " + str(len(dirList)) + " total files in " + pdfDir

    return depends

def findepend_0(pdf, pdfDir, pdfList, allList, depends):
    if not os.path.exists(pdfDir + "/" + pdf + ".pdf"):
        return depends

    thisDepends = []
    for entry in allList:
        for line in open(pdfDir + "/" + pdf + ".pdf"):
            if entry in line and not entry in thisDepends:
                thisDepends.append(entry)

    for entry in thisDepends:
        if entry not in depends:
            depends.append(entry)
            depends = findepend_0(entry, pdfDir, pdfList, allList, depends)

    return depends

if __name__ == "__main__":
    import sys
    if len(sys.argv) == 3:
        print findepend(sys.argv[1], sys.argv[2])
    else:
        print "usage: python findepend.py pdfWithoutSuffix pdfDir"
        print "  e.g. python findepend.py ikqbcall2 /data/afids/install/afids/vdev"
