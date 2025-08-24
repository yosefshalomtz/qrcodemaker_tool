#include <iostream>
#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QSvgRenderer>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QMenuBar>
#include <QMainWindow>
#include <QActionGroup>
#include <QInputDialog>

#include "qrcodegen.hpp"

bool ifCircles = false;
int qr_error_level = 3;
int qr_border = 2;
QString defaultQRpath = "https://github.com/yosefshalomtz/qrcodemaker_tool.git";

// void outputText(QString text) {std::cout << text.toStdString() << std::endl;}

const QByteArray getSvgDataFromText(QString text)
{
    if(text.isEmpty()) text = defaultQRpath;
    return QString::fromUtf8(qrcodegen::QrCode::encodeText(text.toStdString().c_str(), (qrcodegen::QrCode::Ecc)qr_error_level).toSvgString(qr_border, ifCircles).c_str()).toUtf8();
}

class SvgWidget : public QWidget
{
    QSvgRenderer renderer;

public:
    SvgWidget(const QByteArray &svgData, QWidget *parent = nullptr)
        : QWidget(parent), renderer(svgData) {}

    void setSvgData(const QByteArray &data)
    {
        renderer.load(data);
        update();
    }

protected:
    void paintEvent(QPaintEvent *) override
    {
        QPainter painter(this);
        renderer.render(&painter);
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/icon.png"));

    QMainWindow window;
    QWidget centralWidget;
    window.setCentralWidget(&centralWidget);
    QVBoxLayout *layout = new QVBoxLayout(&centralWidget);
    QMenuBar* menuBar = new QMenuBar(&window);
    QMenu* optionMenu = menuBar->addMenu("option");

    QMenu* bits_style_menu = optionMenu->addMenu("bits style");
    QActionGroup* bits_style_menu_actions = new QActionGroup(bits_style_menu);
    QAction* bits_style_menu_rectagles = bits_style_menu->addAction("rectagles");
    QAction* bits_style_menu_circles = bits_style_menu->addAction("circles");
    bits_style_menu_rectagles->setCheckable(true);
    bits_style_menu_rectagles->setChecked(true);
    bits_style_menu_circles->setCheckable(true);
    bits_style_menu_actions->addAction(bits_style_menu_rectagles);
    bits_style_menu_actions->addAction(bits_style_menu_circles);
    bits_style_menu_actions->setExclusive(true);
    optionMenu->addSeparator();

    QAction* border_selector = optionMenu->addAction("border");
    optionMenu->addSeparator();

    QMenu* ecc_level_menu = optionMenu->addMenu("ECC level");
    QActionGroup* ecc_level_menu_actions = new QActionGroup(ecc_level_menu);
    QAction* ecc_level_menu_high = ecc_level_menu->addAction("HIGH");
    QAction* ecc_level_menu_quartile = ecc_level_menu->addAction("QUARTILE");
    QAction* ecc_level_menu_medium = ecc_level_menu->addAction("MEDIUM");
    QAction* ecc_level_menu_low = ecc_level_menu->addAction("LOW");
    ecc_level_menu_high->setCheckable(true);
    ecc_level_menu_quartile->setCheckable(true);
    ecc_level_menu_medium->setCheckable(true);
    ecc_level_menu_low->setCheckable(true);
    ecc_level_menu_high->setChecked(true);
    ecc_level_menu_actions->addAction(ecc_level_menu_high);
    ecc_level_menu_actions->addAction(ecc_level_menu_quartile);
    ecc_level_menu_actions->addAction(ecc_level_menu_medium);
    ecc_level_menu_actions->addAction(ecc_level_menu_low);
    ecc_level_menu_actions->setExclusive(true);
    optionMenu->addSeparator();

    window.setMenuBar(menuBar);

    QLineEdit *textToEncodeLineEdit = new QLineEdit;
    textToEncodeLineEdit->setPlaceholderText("enter text to encode...");
    layout->addWidget(textToEncodeLineEdit);

    SvgWidget *svgWidget = new SvgWidget(QByteArray(getSvgDataFromText(defaultQRpath)));
    layout->addWidget(svgWidget);

    QObject::connect(border_selector, &QAction::triggered, [&]() {
        bool ok;
        int value = QInputDialog::getInt(nullptr, "Border Size", "Enter border size:", qr_border, 0, 100, 1, &ok);
        if(ok) qr_border = value;
        svgWidget->setSvgData(getSvgDataFromText(textToEncodeLineEdit->text()));
    });

    QObject::connect(bits_style_menu_rectagles, &QAction::triggered, [&]()
    {
        ifCircles = false;
        svgWidget->setSvgData(getSvgDataFromText(textToEncodeLineEdit->text()));
    });
    QObject::connect(bits_style_menu_circles, &QAction::triggered, [&]()
    {
        ifCircles = true;
        svgWidget->setSvgData(getSvgDataFromText(textToEncodeLineEdit->text()));
    });

    QObject::connect(ecc_level_menu_high, &QAction::triggered, [&]()
    {
        qr_error_level = 3;
        svgWidget->setSvgData(getSvgDataFromText(textToEncodeLineEdit->text()));
    });
    QObject::connect(ecc_level_menu_quartile, &QAction::triggered, [&]()
    {
        qr_error_level = 2;
        svgWidget->setSvgData(getSvgDataFromText(textToEncodeLineEdit->text()));
    });
    QObject::connect(ecc_level_menu_medium, &QAction::triggered, [&]()
    {
        qr_error_level = 1;
        svgWidget->setSvgData(getSvgDataFromText(textToEncodeLineEdit->text()));
    });
    QObject::connect(ecc_level_menu_low, &QAction::triggered, [&]()
    {
        qr_error_level = 0;
        svgWidget->setSvgData(getSvgDataFromText(textToEncodeLineEdit->text()));
    });

    // text to qr line edit listener
    QObject::connect(textToEncodeLineEdit, &QLineEdit::textChanged, [&](const QString &text)
    {
        svgWidget->setSvgData(getSvgDataFromText(text));
    });

    window.resize(500, 500);
    window.show();

    return app.exec();
}
