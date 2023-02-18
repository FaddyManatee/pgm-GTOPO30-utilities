typedef struct DEM gtopoDEM;

gtopoDEM* createDEM(int width, int height);
int getWidth(gtopoDEM *targetDEM);
int getHeight(gtopoDEM *targetDEM);
signed short** getRaster(gtopoDEM *targetDEM);
signed short getElevation(gtopoDEM *targetDEM, int row, int column);
void setElevation(gtopoDEM *targetDEM, signed short value, int row, int column);
void freeDEM(gtopoDEM *image);
