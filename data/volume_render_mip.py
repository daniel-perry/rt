#!/usr/bin/env python
import sys
import vtk

def main(argv):
  if len(argv) < 2:
    print "usage:",argv[0]," data.nrrd data.cmap"
    exit(1)
  data_fn = argv[1]
  cmap_fn = argv[2]
  reader = vtk.vtkPNrrdReader()
  reader.SetFileName(data_fn)
  reader.Update()
  data = reader.GetOutput()
  # opacity function
  opacityFunction = vtk.vtkPiecewiseFunction()
  # color function
  colorFunction = vtk.vtkColorTransferFunction()
  cmap = open(cmap_fn, 'r')
  for line in cmap.readlines():
    parts = line.split()
    value = float(parts[0])
    r = float(parts[1])
    g = float(parts[2])
    b = float(parts[3])
    a = float(parts[4])
    opacityFunction.AddPoint(value, a)
    colorFunction.AddRGBPoint(value, r, g, b)
  # volume setup:
  volumeProperty = vtk.vtkVolumeProperty()
  volumeProperty.SetColor(colorFunction)
  volumeProperty.SetScalarOpacity(opacityFunction)
  # composite function (using ray tracing)
  compositeFunction = vtk.vtkVolumeRayCastMIPFunction()
  volumeMapper = vtk.vtkVolumeRayCastMapper()
  volumeMapper.SetVolumeRayCastFunction(compositeFunction)
  volumeMapper.SetInput(data)
  # make the volume
  volume = vtk.vtkVolume()
  volume.SetMapper(volumeMapper)
  volume.SetProperty(volumeProperty)
  # renderer
  renderer = vtk.vtkRenderer()
  renderWin = vtk.vtkRenderWindow()
  renderWin.AddRenderer(renderer)
  renderInteractor = vtk.vtkRenderWindowInteractor()
  renderInteractor.SetRenderWindow(renderWin)
  renderer.AddVolume(volume)
  renderer.SetBackground(0,0,0)
  renderWin.SetSize(400, 400)
  renderInteractor.Initialize()
  renderWin.Render()
  renderInteractor.Start()

if __name__ == '__main__':
  main(sys.argv)
