#include <QtGui>
#include <QApplication>

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>


class QtSAR :public QMainWindow
{
public:
	QtSAR(QWidget *parent = 0);
	virtual ~QtSAR();
};

QtSAR::QtSAR(QWidget *parent)
		: QMainWindow(parent)
{
	this->resize(500, 20);
	QLabel *label = new QLabel(this);
	label->setText("Start AutoSAR!");
	this->setCentralWidget(label);
	this->setWindowTitle(QApplication::translate("Main", "OpenSAR Console", 0, QApplication::UnicodeUTF8));
}

QtSAR::~QtSAR() {
	// TODO Auto-generated destructor stub
}
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtSAR w;
    w.show();
    a.setStyleSheet(QString::fromUtf8("font: 12pt \"Consolas\";"));
    return a.exec();
}
