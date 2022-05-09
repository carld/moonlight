SUBDIRS=

SRC= CommonMath.C Vector2.C Vector.C \
 Matrix3x3.C HVector.C Matrix4x4.C Transform.C Quaternion.C \
 IVector.C IVector3.C \
 MLInteger.C IntegerArray.C \
 MLString.C Chrono.C \
 Spectrum.C Color.C Color8.C HSVColor.C HLSColor.C \
 ListFiles.C \
 MLPath.C MLFilename.C MLFileEntry.C MLPathList.C MLCommandLine.C \
 Randy.C poly2tri.C IArray.C \
 Box.C \
 BoundingSphere.C BoundingSphereBuilder.C \
 BoundingCone.C BoundingCylinder.C \
 BoundingHalfPlane.C \
 MLIOProgress.C debugf.C \
 MLGammaCorrection.C \
 MLConfig.C MLConfigReader.C MLConfigWriter.C \
 MLDynamicLibrary.C \
 MLSocketClient.C MLSocketServer.C MLSocketConnection.C \
 MLLog.C MLLogListener.C MLLogPrinter.C 
SRC+=3D/MLVertex.C 3D/MLSVertex.C 3D/MLEdge.C 3D/MLPolygon.C
SRC+=file/MLFile.C file/MLRFile.C file/MLWFile.C file/MLFileBlock.C file/MLRFileBlock.C file/MLWFileBlock.C file/MLFileInfo.C
SRC+=xdr/XDROp.C xdr/XDRFile.C xdr/XDRMem.C
SRC+=zlib/MLLibZ.C
SRC+=thread/MLThread.C

tools_sources=$(SRC)
tools_dlibs=z pthread dl m
tools_slibs=
tools_install=$(prefix)/lib/moonlight

SHARED=tools
