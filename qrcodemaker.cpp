#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QSvgRenderer>
#include <QVBoxLayout>
#include <QLineEdit>
#include "qrcodegen.hpp"

class SvgWidget : public QWidget {
    QSvgRenderer renderer;
public:
    SvgWidget(const QByteArray &svgData, QWidget *parent = nullptr)
        : QWidget(parent), renderer(svgData) {}

    void setSvgData(const QByteArray &data) {
        renderer.load(data);
        update();
    }

protected:
    void paintEvent(QPaintEvent *) override {
        QPainter painter(this);
        renderer.render(&painter);
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/icon.png"));

    QByteArray svgData(R"(
        <svg xmlns="http://www.w3.org/2000/svg" width="100" height="100">
        </svg>
    )");

    QWidget window;
    QVBoxLayout *layout = new QVBoxLayout(&window);

    QLineEdit *lineEdit = new QLineEdit;
    lineEdit->setPlaceholderText("enter text to encode...");
    layout->addWidget(lineEdit);

    SvgWidget *svgWidget = new SvgWidget(svgData);
    layout->addWidget(svgWidget);

    QObject::connect(lineEdit, &QLineEdit::textChanged, [&](const QString &text){
        svgWidget->setSvgData(
            QString::fromUtf8(qrcodegen::QrCode::encodeText(text.toStdString().c_str(), qrcodegen::QrCode::Ecc::HIGH).toSvgString(2).c_str()).toUtf8()
        );
    });

    window.resize(500, 500);
    window.show();

    return app.exec();
}
