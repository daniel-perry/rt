void swap(short& data)
{
  char* p=(char*)&data;
  char tmp=p[0];
  p[0]=p[1];
  p[1]=tmp;
}

PhongVolumeMaterial::PhongVolumeMaterial(const std::string& headername,
                                         const std::string& cmapname,
                                         const Point& lower, const Point& upper,
                                         double grid_stepsize, float maxopacity,
                                         float Kd, float Ka,
                                         const Color& phong_color,
                                         int phong_exponent)
  : cmap(cmapname), lower(lower), upper(upper), grid_stepsize(grid_stepsize),
    maxopacity(maxopacity), Kd(Kd), Ka(Ka), phong_color(phong_color),
    phong_exponent(phong_exponent)
{
  diag = upper-lower;

  ifstream hdr(headername.c_str());
  string volumename;
  hdr >> volumename;
  int nx, ny, nz;
  hdr >> nx >> ny >> nz;
  short offsetvalue;
  hdr >> offsetvalue;
  string fileendian;
  hdr >> fileendian;
  if(!hdr){
    cerr << "Error reading header: " << headername << '\n';
    exit(1);
  }
  if(fileendian != "BigEndian" && fileendian != "LittleEndian"){
    cerr << "Bad data endian: " << fileendian << " in " << headername << "\n";
    exit(1);
  }
  cerr << "Reading " << volumename << ": " << nx << 'x' << ny << 'x' << nz << '\n';
  data.resize(nx, ny, nz);
  cellsize = diag * Vector(1./(nx-1), 1./(ny-1), 1./(nz-1));
  ifstream in(volumename.c_str()); //
  in.read(reinterpret_cast<char*>(&data(0,0,0)), nx*ny*nz*sizeof(short));
  if(!in){
    cerr << "Error reading data: " << volumename << '\n';
    exit(1);
  }

  double world_stepsize = cellsize.length()/pow(3, 1./3.) * grid_stepsize;
  cmap.rescale(world_stepsize);

  short tmp = 0x1234;
  string machineendian;
  if(*reinterpret_cast<char*>(&tmp) == 0x12)
    machineendian = "BigEndian";
  else
    machineendian = "LittleEndian";
  if(machineendian != fileendian){
    for(int i=0;i<nz;i++){
      for(int j=0;j<ny;j++){
        for(int k=0;k<nx;k++){
          swap(data(k,j,i));
          data(k,j,i) += offsetvalue;
        }
      }
    }
  } else {
    for(int i=0;i<nz;i++){
      for(int j=0;j<ny;j++){
        for(int k=0;k<nx;k++){
          data(k,j,i) += offsetvalue;
        }
      }
    }
  }
}
