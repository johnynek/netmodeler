#include <netmodeler.h>

using namespace Starsky;
using namespace std;

int main(int argc, char* argv[])
{
  Ran1Random r(-1);
#if 1
  //Incrementable networks
  //UniformAttachNet my_net(RandomNetwork(7, 0.5, r), r, 2);
  PrefAtNetwork my_net(RandomNetwork(3,1,r),r,2);
	       
  int triangles, wedges;
  cout << "#size triangles wedges" << endl;
  while(my_net.getNodes().size() < 5000) {
    my_net.incrementTime(100);
    my_net.getTrianglesWedges(triangles, wedges);
    cout << my_net.getNodes().size() << " " << triangles << " " << wedges << endl;
  }
#else
  //Static networks:
  cout << "#size triangles wedges" << endl;
  int triangles, wedges;
  for(int i = 100; i < 5000; i+=100) {
    ExpProbabilityFunction expl(2.0/3.0,2, i);
    DegreeLawRandomNetwork my_net(i,expl,r,true);
    my_net.getTrianglesWedges(triangles, wedges);
    cout << my_net.getNodes().size() << " " << triangles << " " << wedges << endl;
  }
#endif
}
