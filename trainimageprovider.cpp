#include "trainimageprovider.h"

TrainImageProvider::TrainImageProvider(QObject *parent) : QObject(parent), QQuickImageProvider(QQuickImageProvider::Pixmap)
{
    this->pixmap = QPixmap(500, 400);
    this->pixmap.fill(QColor("#303030").rgba());
}

/*!
 * \brief TrainImageProvider::requestPixmap
 * \param id
 * \param size
 * \param requestedSize
 * \return
 */
QPixmap TrainImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    Q_UNUSED(id);

    int width = 100;
    int height = 100;

    const QPixmap *resultPixmap;

    if (this->pixmap.isNull()) {
        this->pixmap = QPixmap(requestedSize.width() > 0 ? requestedSize.width() : width,
                      requestedSize.height() > 0 ? requestedSize.height() : height);
        this->pixmap.fill(QColor("#303030").rgba());
    }

    resultPixmap = &(this->pixmap);

    if (size)
        *size = QSize(resultPixmap->width(), resultPixmap->height());

    return *resultPixmap;
}

/*!
 * \brief TrainImageProvider::updatePixmap
 * \param input
 * \param result
 */
void TrainImageProvider::updatePixmap(QPixmap *input) {

    this->pixmap = QPixmap(*input);

    emit imageUpdated();
}

/*!
 * \brief TrainImageProvider::getImageHSI
 * \param x
 * \param y
 * \return
 */
void TrainImageProvider::getImageHSI(int x, int y, int *colorComponents) {

    int hue = 0, saturation = 0, intensity = 0;

    if ( ! this->pixmap.isNull() ) {

        QImage image = this->pixmap.toImage();

        if (x >= 0 && y >=0 && x < image.width() && y < image.height() ) {
            QColor color = image.pixelColor(x, y);
            hue = color.hslHueF() * 255;
            saturation = color.hslSaturationF() * 255;
            intensity = color.lightnessF() * 255;
        }
    }

    // int *result = new int(3);
    colorComponents[0] = hue;
    colorComponents[1] = saturation;
    colorComponents[2] = intensity;
}
