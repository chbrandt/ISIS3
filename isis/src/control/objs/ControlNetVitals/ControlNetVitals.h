#ifndef ControlNetVitals_h
#define ControlNetVitals_h
/**
 * @file
 * $Revision: 1.2 $
 * $Date: 2010/06/28 17:15:01 $
 *
 *   Unless noted otherwise, the portions of Isis written by the USGS are
 *   public domain. See individual third-party library and package descriptions
 *   for intellectual property information, user agreements, and related
 *   information.
 *
 *   Although Isis has been used by the USGS, no warranty, expressed or
 *   implied, is made by the USGS as to the accuracy and functioning of such
 *   software and related material nor shall the fact of distribution
 *   constitute any such warranty, and no responsibility is assumed by the
 *   USGS in connection therewith.
 *
 *   For additional information, launch
 *   $ISISROOT/doc/documents/Disclaimers/Disclaimers.html
 *   in a browser or see the Privacy &amp; Disclaimers page on the Isis website,
 *   http://isis.astrogeology.usgs.gov, and the USGS privacy and disclaimers on
 *   http://www.usgs.gov/privacy.html.
 */

#include <QStringList>

#include "ControlMeasure.h"
#include "ControlNet.h"
#include "ControlPoint.h"

namespace Isis {
  class ControlNet;


  /**
  *
  *  @brief ControlNetVitals
  *
  *  This class is designed to represent the health of a control network.
  *  It utilizes signals and slots to listen for changes in an observed Control Network and
  *  re-evaluates the health of a network whenever a change is made.
  *  It tracks several statistics, and is intended to be the back-end for the ControlHealthMonitorWidget
  *  that is located in IPCE.
  *
  *  The ControlNetVitals class keeps track of several member variables that are a running counter
  *  for network statistics in regard to the health of the observed network. It creates these
  *  variables upon intialization and references these internal variables when returning certain
  *  statistics about a Control Network that can't be accessed by wrapper methods for the network itself.
  *  It then listens for specific signals to be emitted whenever a change is made to the network
  *  to update it's internal counters with respect to that change.
  *
  *  @author 2018-05-28 Adam Goins
  *
  *  @internal
  *    @history 2018-05-28 Adam Goins - Initial Creation.
  *    @history 2018-06-14 Adam Goins & Jesse Maple - Refactored method calls and Signal/Slot usage.
  *    @history 2018-06-15 Adam Goins - Added documentation.
  *    @history 2018-06-25 Kristin Berry - Fixed problem with getImagesBelowMeasureThreshold().size()
  *                           not matching numImagesBelowMeasureThreshold(). Fixed a similar
  *                           problem with numPointsBelowMeasureThreshold().
  */
  class ControlNetVitals : public QObject {
    Q_OBJECT

    public:
      ControlNetVitals(ControlNet *net);
      virtual ~ControlNetVitals();

      void initializeVitals();

      bool hasIslands();
      int numIslands();
      const QList< QList<QString> > &getIslands();

      ControlPoint *getPoint(QString id);

      int numPoints();
      int numIgnoredPoints();
      int numLockedPoints();
      int numFixedPoints();
      int numConstrainedPoints();
      int numFreePoints();
      int numPointsBelowMeasureThreshold(int num=3);

      int numImages();
      int numMeasures();
      int numImagesBelowMeasureThreshold(int num=3);
      int numImagesBelowHullTolerance(int tolerance=75);

      QList<QString> getCubeSerials();
      QList<ControlPoint*> getAllPoints();
      QList<ControlPoint*> getIgnoredPoints();
      QList<ControlPoint*> getLockedPoints();
      QList<ControlPoint*> getFixedPoints();
      QList<ControlPoint*> getConstrainedPoints();
      QList<ControlPoint*> getFreePoints();
      QList<ControlPoint*> getPointsBelowMeasureThreshold(int num=3);

      QList<QString> getImagesBelowMeasureThreshold(int num=3);
      QList<QString> getImagesBelowHullTolerance(int num=75);

      QString getNetworkId();
      QString getStatus();
      QString getStatusDetails();

      void emitHistoryEntry(QString entry, QString id, QVariant oldValue, QVariant newValue);

    signals:
      void networkChanged();
      void historyEntry(QString, QString, QVariant, QVariant, QString);

    public slots:
      void validate();
      void validateNetwork(ControlNet::ModType);
      void addPoint(ControlPoint *);
      void pointModified(ControlPoint *, ControlPoint::ModType, QVariant, QVariant);
      void deletePoint(ControlPoint *);
      void addMeasure(ControlMeasure *);
      void measureModified(ControlMeasure *, ControlMeasure::ModType, QVariant, QVariant);
      void deleteMeasure(ControlMeasure *);


    private:
      //! The Control Network that the vitals class is observing.
      ControlNet *m_controlNet;

      //! The string representing the status of the net. Healthy, Weak, or Broken.
      QString m_status;
      //! The string providing details into the status of the network.
      QString m_statusDetails;

      //! A QList containing every island in the net. Each island consists of a QList containing
      //! All cube serials for that island.
      QList< QList< QString > > m_islandList;

      //! The measureCount maps track how many points/images have how many measures.
      //! For instance, if I wanted to know how many points have 3 measures I would query
      //! the m_pointMeasureCounts with a key of 3 and it would return how many points
      //! have 3 measures.
      QMap<int, int> m_pointMeasureCounts;
      //! The same is true for imageMeasureCounts, except for images.
      QMap<int, int> m_imageMeasureCounts;

      //! The pointTypeCounts operates in the same fashion as the above two, except
      //! that the key would be the ControlPoint::PointType you're searching for.
      //! For instance, if I wanted to know how many points were fixed I would query
      //! This map at key ControlPoint::Fixed and it would return how many fixed points there are.
      QMap<ControlPoint::PointType, int> m_pointTypeCounts;

      //! The number of points in the network.
      int m_numPoints;
      //! The number of ignored points in the network.
      int m_numPointsIgnored;
      //! The number of edit locked points in the network.
      int m_numPointsLocked;
      //! The number of measures in the network.
      int m_numMeasures;
  };
};

#endif