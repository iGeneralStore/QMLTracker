#include "filter.h"
#include "opencvhelper.h"
#include "rgbframehelper.h"

#include <QFile>

QVideoFilterRunnable* Filter::createFilterRunnable() {
    return new FilterRunnable(this);
}

FilterRunnable::FilterRunnable(Filter *filter)
    : m_filter(filter),
      m_classifier(0),
      isTracked(false)
{
    m_filter->isAdded = false;

}

QVideoFrame FilterRunnable::run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags) {
    Q_UNUSED(surfaceFormat);
    Q_UNUSED(flags);

    // Supports YUV (I420 and NV12) and RGB. The GL path is readback-based and slow.
    if (!input->isValid()
            || (input->handleType() != QAbstractVideoBuffer::NoHandle && input->handleType() != QAbstractVideoBuffer::GLTextureHandle)) {
        qWarning("Invalid input format");
        return QVideoFrame();
    }

    input->map(QAbstractVideoBuffer::ReadOnly);
    if ( input->pixelFormat() == QVideoFrame::Format_YUV420P || input->pixelFormat() == QVideoFrame::Format_NV12 ) {
        m_yuv = true;
        m_mat = yuvFrameToMat8(*input);
    } else {
        m_yuv = false;
        QImage wrapper = imageWrapper(*input);
        if (wrapper.isNull()) {
            if (input->handleType() == QAbstractVideoBuffer::NoHandle)
                input->unmap();
            return *input;
        }
        m_mat = imageToMat8(wrapper);
    }
    ensureC3(&m_mat);
    if (input->handleType() == QAbstractVideoBuffer::NoHandle)
        input->unmap();

    // 2. Flip if requested
    if(true) {
        cv::flip(m_mat, m_mat, 1);
    }

    imgPrev = imgCurrent;

    imgCurrent = mat8ToImage(m_mat);

    process();


    if(_frameOutput.type() == CV_8UC1)
        cvtColor(_frameOutput, _frameOutput, CV_GRAY2BGR);

    return QVideoFrame(mat8ToImage(_frameOutput));
}

void FilterRunnable::process() {

    cv::Size winSize(31,31);

    _framePrev = imageToMat8(imgPrev);
    _frameCurrent = imageToMat8(imgCurrent);
    _frameOutput = imageToMat8(imgCurrent);

    if(_framePrev.empty() || _frameCurrent.empty())
        return;

    cvtColor(_framePrev, _framePrev, cv::COLOR_BGR2GRAY);
    cvtColor(_frameCurrent, _frameCurrent, cv::COLOR_BGR2GRAY);

    if(!isTracked) {
        isTracked = true;
        cv::goodFeaturesToTrack(_frameCurrent, points[1], MAX_CORNERS, 0.01, 10, cv::Mat(), 3, 0, 0.04);
        swap(points[1], points[0]);
    }

    if(m_filter->isAdded) {
        points[1].push_back(m_filter->point);
        swap(points[1], points[0]);
        m_filter->isAdded = false;
    }

    cv::calcOpticalFlowPyrLK(
                _framePrev,
                _frameCurrent,
                points[0],
                points[1],
                status,
                err,
                winSize,
                3,
                cv::TermCriteria(cv::TermCriteria::COUNT | cv::TermCriteria::EPS, 20, 0.03),
                0,
                0.001
    );

    for(int i=0;i<points[1].size();i++) {
        cv::circle(_frameOutput, points[1][i], 15, cv::Scalar(0,255,0), -1, 8);
        cv::line(_frameOutput, points[0][i], points[1][i], cv::Scalar(255,0,0));
    }

    swap(points[1], points[0]);
}
