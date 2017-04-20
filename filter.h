#ifndef FILTER_H
#define FILTER_H

#include <QAbstractVideoFilter>

#include <opencv2/core/core.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/objdetect/objdetect.hpp>

#include <QDebug>
#include <QCursor>

const int MAX_CORNERS = 500;
const int MAX_POINTS = 10000;

using namespace std;

class Filter : public QAbstractVideoFilter
{
    Q_OBJECT
public:
    QVideoFilterRunnable *createFilterRunnable() Q_DECL_OVERRIDE;

    Q_INVOKABLE void addPoint(float x, float y) { point = cv::Point2f(x,y); isAdded = true; }
    //Q_INVOKABLE void addPoint(float x, float y) { point = cv::Point2f(QCursor::pos().x(),QCursor::pos().y()); isAdded = true; }

signals:
    void finished(QObject *e);

private:
    cv::Point2f point;
    bool isAdded;

    friend class FilterRunnable;
};

class FilterRunnable : public QVideoFilterRunnable {
public:
    FilterRunnable(Filter *filter);
    QVideoFrame run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags) Q_DECL_OVERRIDE;

private:
    Filter *m_filter;
    cv::Mat m_mat;
    cv::Mat _framePrev, _frameCurrent, _frameOutput;
    bool m_yuv;

    cv::CascadeClassifier *m_classifier;
    QString m_prevName;

    bool isTracked;
    vector<cv::Point2f> points[2];
    cv::Point2f point;

    vector<uchar> status;
    vector<float> err;

    QImage imgPrev;
    QImage imgCurrent;



    void process(); // added by KH
};

class FilterResult : public QObject {

    Q_OBJECT

public slots:
    QVariantList rects() const { return m_rects; }

private:
    QVariantList m_rects;
    friend class FilterRunnable;
};

#endif // FILTER_H
