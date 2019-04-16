#include "LoadingPopup.h"
#include "LoadingWidget.h"

#include <QLabel>
#include <QKeyEvent>

#define POPUP_WIDTH 108
#define POPUP_HEIGHT 88

LoadingPopup::LoadingPopup(QWidget* parent)
    : QDialog(parent, Qt::CustomizeWindowHint)
    , m_loadingWidget(new LoadingWidget(this))
    , m_textLabel(new QLabel(this))
{
    init();
}

LoadingPopup::~LoadingPopup()
{
    delete m_textLabel;
    delete m_loadingWidget;
}

void LoadingPopup::init()
{
    resize(POPUP_WIDTH, POPUP_HEIGHT);
    setMinimumSize(POPUP_WIDTH, POPUP_HEIGHT);
    setMaximumSize(POPUP_WIDTH, POPUP_HEIGHT);
    setAutoFillBackground(true);
    setWindowOpacity(0.8);

    QPalette palette;
    palette.setColor(QPalette::Window, Qt::black);
    setPalette(palette);

    m_loadingWidget->start(":Utils/Loading48x48.gif");
    m_loadingWidget->setGeometry(30, 10, 48, 48);

    m_textLabel->setGeometry(0, 64, POPUP_WIDTH, 20);
    m_textLabel->setAlignment(Qt::AlignCenter);
    palette.setColor(QPalette::WindowText, Qt::white);
    m_textLabel->setPalette(palette);
}

void LoadingPopup::keyPressEvent(QKeyEvent* event)
{
    if (event->key() != Qt::Key_Escape)
        QDialog::keyPressEvent(event);
}

void LoadingPopup::setText(const QString& text)
{
    m_textLabel->setText(text);
}
