/****************************************************************************
**
** Copyright (c) 2008-2020 C.B. Barber. All rights reserved.
** $Id: //main/2019/qhull/src/qhulltest/QhullFacet_test.cpp#6 $$Change: 3006 $
** $DateTime: 2020/07/29 18:28:16 $$Author: bbarber $
**
****************************************************************************/

//pre-compiled headers
#include <iostream>
#include "qhulltest/RoadTest.h" // QT_VERSION

#include "libqhullcpp/QhullFacet.h"
#include "libqhullcpp/QhullError.h"
#include "libqhullcpp/Coordinates.h"
#include "libqhullcpp/RboxPoints.h"
#include "libqhullcpp/QhullFacetList.h"
#include "libqhullcpp/QhullFacetSet.h"
#include "libqhullcpp/QhullPointSet.h"
#include "libqhullcpp/QhullRidge.h"
#include "libqhullcpp/Qhull.h"

using std::cout;
using std::endl;
using std::ostringstream;
using std::ostream;
using std::string;

namespace orgQhull {

class QhullFacet_test : public RoadTest
{
    Q_OBJECT

#//!\name Test slots
private slots:
    void cleanup();
    void t_construct_qh();
    void t_constructConvert();
    void t_getSet();
    void t_getSet2d();
    void t_value();
    void t_foreach();
    void t_io();
};//QhullFacet_test

void
add_QhullFacet_test()
{
    new QhullFacet_test();  // RoadTest::s_testcases
}

//Executed after each testcase
void QhullFacet_test::
cleanup()
{
    RoadTest::cleanup();
}

void QhullFacet_test::
t_construct_qh()
{
    // Qhull.runQhull() constructs QhullFacets as facetT
    QhullQh qh;
    QhullFacet f(&qh);
    QVERIFY(!f.isValid());
    QCOMPARE(f.dimension(),0);
}//t_construct_qh

void QhullFacet_test::
t_constructConvert()
{
    // Qhull.runQhull() constructs QhullFacets as facetT
    Qhull q2;
    QhullFacet f(q2);
    QVERIFY(!f.isValid());
    QCOMPARE(f.dimension(),0);
    RboxPoints rcube("c");
    Qhull q(rcube,"Qt QR0");  // triangulation of rotated unit cube
    QhullFacet f2(q.beginFacet());
    QCOMPARE(f2.dimension(),3);
    f= f2; // copy assignment
    QVERIFY(f.isValid());
    QCOMPARE(f.dimension(),3);
    QhullFacet f5= f2;
    QVERIFY(f5==f2);
    QVERIFY(f5==f);
    QhullFacet f3(q, f2.getFacetT());
    QCOMPARE(f,f3);
    QhullFacet f4(q, f2.getBaseT());
    QCOMPARE(f,f4);
}//t_constructConvert

void QhullFacet_test::
t_getSet()
{
    RboxPoints rcube("c");
    {
        Qhull q(rcube,"Qt QR0");  // triangulation of rotated unit cube
        cout << " rbox c | qhull Qt QR0 QR" << q.rotateRandom() << "   distanceEpsilon " << q.distanceEpsilon() << endl;
        QCOMPARE(q.facetCount(), 12);
        QCOMPARE(q.vertexCount(), 8);
        QhullFacetListIterator i(q.facetList());
        while(i.hasNext()){
            const QhullFacet f= i.next();
            cout << f.id() << endl;
            QhullFacet f2;
            f2.setFacetT(f.qh(), f.getFacetT());
            QCOMPARE(f, f2);
            QCOMPARE(f.dimension(),3);
            QVERIFY(f.id()>0 && f.id()<=39);
            QVERIFY(f.isValid());
            if(i.hasNext()){
                QCOMPARE(f.next(), i.peekNext());
                QVERIFY(f.next()!=f);
                QCOMPARE(f.next().previous(), f);
                QVERIFY(f.hasNext());
                QVERIFY(f.next().hasPrevious());
            }else
              QVERIFY(!f.hasNext());
            QVERIFY(i.hasPrevious());
            QCOMPARE(f, i.peekPrevious());
        }
        while(i.hasPrevious()){
          const QhullFacet f= i.previous();
          cout << f.id() << endl;
          QVERIFY(f.isValid());
          if(i.hasPrevious()){
            QVERIFY(f.hasPrevious());
            QCOMPARE(f.previous(), i.peekPrevious());
            QVERIFY(f.previous()!=f);
            QVERIFY(f.previous().hasNext());
            QCOMPARE(f.previous().next(), f);
          }else
            QVERIFY(!f.hasPrevious());
          QVERIFY(i.hasNext());
          QCOMPARE(f, i.peekNext());
        }
        // test tricoplanarOwner
        QhullFacet facet= q.beginFacet();
        QhullFacet tricoplanarOwner= facet.tricoplanarOwner();
        int tricoplanarCount= 0;
        i.toFront();
        while(i.hasNext()){
            const QhullFacet f= i.next();
            if(f.tricoplanarOwner()==tricoplanarOwner){
                tricoplanarCount++;
            }
        }
        QCOMPARE(tricoplanarCount, 2);
        int tricoplanarCount2= 0;
        foreach (QhullFacet f, q.facetList()){  // Qt only
            QhullHyperplane hi= f.innerplane();
            QCOMPARE(hi.count(), 3);
            double innerOffset= hi.offset() + 0.5;
            cout << "InnerPlane: " << hi << "   innerOffset+0.5 " << innerOffset << endl;
            QhullHyperplane h= f.hyperplane();
            double offset= h.offset() + 0.5;
            cout << "Hyperplane: " << h << "   offset+0.5 " << offset << endl;
            QCOMPARE(h.count(), 3);
            QCOMPARE(h.offset(), -0.5);
            double n= h.norm();
            QCOMPARE(n, 1.0);
            QhullHyperplane ho= f.outerplane();
            QCOMPARE(ho.count(), 3);
            double outerOffset= ho.offset()+0.5;
            cout << "OuterPlane: " << ho << "   outerOffset+0.5 " << outerOffset << endl;
            QVERIFY(offset < innerOffset); // the outerOffset is more negative than the innerOffset.  The expected values are -0.5 for the unit cube centered at the origin
            QVERIFY(outerOffset < offset); // QR1595623290 was 8.4x,   innerOffset+0.5 9.71445e-15, 2.5e-16 max. distance of a new vertex to a facet
            QVERIFY2(fabs(innerOffset) < (10 * q.distanceEpsilon()), "Guessed epsilon from -0.5 due to rotation");   
            QVERIFY2(fabs(outerOffset) < (10 * q.distanceEpsilon()), "Guessed epsilon from -0.5 due to rotation");

            for(int k= 0; k<3; k++){
                QVERIFY(ho[k]==hi[k]);
                QVERIFY(ho[k]==h[k]);
            }
            QhullPoint center= f.getCenter();
            cout << "Center: " << center;
            double d= f.distance(center);
            QVERIFY(d < innerOffset-outerOffset);
            QhullPoint center2= f.getCenter(qh_PRINTcentrums);
            QCOMPARE(center, center2);
            if(f.tricoplanarOwner()==tricoplanarOwner){
                tricoplanarCount2++;
            }
            cout << endl;
        }
        QCOMPARE(tricoplanarCount2, 2);
        Qhull q2(rcube,"d Qz Qt QR0");  // 3-d triangulation of Delaunay triangulation (the cube)
        cout << " rbox c | qhull d Qz Qt QR0 QR" << q2.rotateRandom() << "   distanceEpsilon " << q2.distanceEpsilon() << endl;
        QhullFacet f2= q2.firstFacet();
        QhullPoint center3= f2.getCenter(qh_PRINTtriangles);
        QCOMPARE(center3.dimension(), 3);
        QhullPoint center4= f2.getCenter();
        QCOMPARE(center4.dimension(), 4);
        for(int k= 0; k<3; k++){
            QVERIFY(center4[k]==center3[k]);
        }
        Qhull q3(rcube,"v Qz QR0");  // Voronoi diagram of a cube (one vertex)
        cout << " rbox c | qhull v Qz QR0 QR" << q3.rotateRandom() << "   distanceEpsilon " << q3.distanceEpsilon() << endl;

        q3.setFactorEpsilon(400); // Voronoi vertices are not necessarily within distance episilon
        QhullPoint origin= q3.inputOrigin();
        int voronoiCount= 0;
        foreach(QhullFacet f, q3.facetList()){ //Qt only
            if(f.isGood()){
                ++voronoiCount;
                QhullPoint p= f.voronoiVertex();
                cout << p.print("Voronoi vertex: ")
                    << " Is it within " << q3.factorEpsilon() << " * distanceEpsilon (" << q3.distanceEpsilon() << ") of the origin?" << endl;
                QCOMPARE(p, origin);
            }
        }
        QCOMPARE(voronoiCount, 1);
    }
}//t_getSet

void QhullFacet_test::
t_getSet2d()
{
    RboxPoints rsquare("c D2");
    Qhull q(rsquare, "o");  // convex hull of square
    q.setOutputStream(&cout);
    cout << "Points and facets.  Facet vertices in counter-clockwise order (option 'o')\n";
    q.outputQhull();
    int n= q.facetCount();
    QhullFacet f= q.firstFacet();
    QhullVertex v;
    cout << "Facets and vertices in counter-clockwise order (f.nextFacet2d)\n";
    for(int i= 0; i<n; ++i){
        f= f.nextFacet2d(&v);
        cout << "f" << f.id() << " v" << v.id() << " p" << v.point().id() << "\n";
    }
    cout << "Extreme points in counter-clockwise order (option 'Fx')\n";
    q.outputQhull("Fx");
}//t_getSet2d

void QhullFacet_test::
t_value()
{
    RboxPoints rcube("c");
    {
        Qhull q(rcube, "");
        coordT c[]= {0.0, 0.0, 0.0};
        foreach (QhullFacet f, q.facetList()){  // Qt only
            double d= f.distance(q.origin());
            QCOMPARE(d, -0.5);
            double d0= f.distance(c);
            QCOMPARE(d0, -0.5);
            double facetArea= f.facetArea();
            QCOMPARE(facetArea, 1.0);
            #if qh_MAXoutside
                double maxoutside= f.getFacetT()->maxoutside;
                QVERIFY(maxoutside<1e-7);
            #endif
        }
    }
}//t_value

void QhullFacet_test::
t_foreach()
{
    RboxPoints rcube("c W0 300");  // cube plus 300 points on its surface
    {
        Qhull q(rcube, "QR0 Qc"); // keep coplanars, thick facet, and rotate the cube
        int coplanarCount= 0;
        foreach(const QhullFacet f, q.facetList()){
            QhullPointSet coplanars= f.coplanarPoints();
            coplanarCount += coplanars.count();
            QhullFacetSet neighbors= f.neighborFacets();
            QCOMPARE(neighbors.count(), 4);
            QhullPointSet outsides= f.outsidePoints();
            QCOMPARE(outsides.count(), 0);
            QhullRidgeSet ridges= f.ridges();
            QCOMPARE(ridges.count(), 4);
            QhullVertexSet vertices= f.vertices();
            QCOMPARE(vertices.count(), 4);
            int ridgeCount= 0;
            QhullRidge r= ridges.first();
            for(int r0= r.id(); ridgeCount==0 || r.id()!=r0; r= r.nextRidge3d(f)){
                ++ridgeCount;
                if(!r.hasNextRidge3d(f)){
                    QFAIL("Unexpected simplicial facet.  They only have ridges to non-simplicial neighbors.");
                }
            }
            QCOMPARE(ridgeCount, 4);
        }
        QCOMPARE(coplanarCount, 300);
    }
}//t_foreach

void QhullFacet_test::
t_io()
{
    RboxPoints rcube("c");
    {
        Qhull q(rcube, "");
        QhullFacet f= q.beginFacet();
        cout << f;
        ostringstream os;
        os << f.print("\nWith a message\n");
        os << "\nPrint header for the same facet\n";
        os << f.printHeader();
        os << "\nPrint each component\n";
        os << f.printFlags("    - flags:");
        os << f.printCenter(qh_PRINTfacets, "    - center: ");
        os << f.printRidges();
        cout << os.str();
        ostringstream os2;
        os2 << f;
        QString facetString2= QString::fromStdString(os2.str());
        facetString2.replace(QRegExp("\\s\\s+"), " ");
        facetString2.replace(QRegExp("seen "), "");
        ostringstream os3;
        q.qh()->setOutputStream(&os3);
        q.outputQhull("f");
        QString facetsString= QString::fromStdString(os3.str());
        QString facetString3= facetsString.mid(facetsString.indexOf("- f1\n"));
        facetString3= facetString3.left(facetString3.indexOf("\n- f")+1);
        facetString3.replace(QRegExp("\\s\\s+"), " ");
        QCOMPARE(facetString2, facetString3);
    }
}//t_io

// toQhullFacet is static_cast only

}//orgQhull

#include "moc/QhullFacet_test.moc"
