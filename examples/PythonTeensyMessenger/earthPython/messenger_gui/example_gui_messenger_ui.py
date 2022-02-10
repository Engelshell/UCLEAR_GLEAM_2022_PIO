# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'mainwindowdPBKZm.ui'
##
## Created by: Qt User Interface Compiler version 6.2.3
##
## WARNING! All changes made in this file will be lost when recompiling UI file!
################################################################################

from PySide6.QtCore import (QCoreApplication, QDate, QDateTime, QLocale,
    QMetaObject, QObject, QPoint, QRect,
    QSize, QTime, QUrl, Qt)
from PySide6.QtGui import (QBrush, QColor, QConicalGradient, QCursor,
    QFont, QFontDatabase, QGradient, QIcon,
    QImage, QKeySequence, QLinearGradient, QPainter,
    QPalette, QPixmap, QRadialGradient, QTransform)
from PySide6.QtWidgets import (QApplication, QComboBox, QHBoxLayout, QLabel,
    QLineEdit, QMainWindow, QMenuBar, QPlainTextEdit,
    QPushButton, QSizePolicy, QSpacerItem, QVBoxLayout,
    QWidget)

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        if not MainWindow.objectName():
            MainWindow.setObjectName(u"MainWindow")
        MainWindow.resize(673, 482)
        self.centralwidget = QWidget(MainWindow)
        self.centralwidget.setObjectName(u"centralwidget")
        self.verticalLayout_7 = QVBoxLayout(self.centralwidget)
        self.verticalLayout_7.setObjectName(u"verticalLayout_7")
        self.verticalLayout = QVBoxLayout()
        self.verticalLayout.setObjectName(u"verticalLayout")
        self.horizontalLayout_4 = QHBoxLayout()
        self.horizontalLayout_4.setObjectName(u"horizontalLayout_4")
        self.verticalLayout_4 = QVBoxLayout()
        self.verticalLayout_4.setObjectName(u"verticalLayout_4")
        self.consoleEdit = QPlainTextEdit(self.centralwidget)
        self.consoleEdit.setObjectName(u"consoleEdit")
        self.consoleEdit.setAcceptDrops(False)
        self.consoleEdit.setUndoRedoEnabled(False)
        self.consoleEdit.setReadOnly(True)

        self.verticalLayout_4.addWidget(self.consoleEdit)


        self.horizontalLayout_4.addLayout(self.verticalLayout_4)


        self.verticalLayout.addLayout(self.horizontalLayout_4)

        self.horizontalLayout_5 = QHBoxLayout()
        self.horizontalLayout_5.setObjectName(u"horizontalLayout_5")
        self.verticalLayout_5 = QVBoxLayout()
        self.verticalLayout_5.setObjectName(u"verticalLayout_5")
        self.horizontalLayout_6 = QHBoxLayout()
        self.horizontalLayout_6.setObjectName(u"horizontalLayout_6")
        self.sendButton = QPushButton(self.centralwidget)
        self.sendButton.setObjectName(u"sendButton")

        self.horizontalLayout_6.addWidget(self.sendButton)

        self.messageEdit = QLineEdit(self.centralwidget)
        self.messageEdit.setObjectName(u"messageEdit")

        self.horizontalLayout_6.addWidget(self.messageEdit)


        self.verticalLayout_5.addLayout(self.horizontalLayout_6)

        self.horizontalLayout = QHBoxLayout()
        self.horizontalLayout.setObjectName(u"horizontalLayout")
        self.serialOpenButton = QPushButton(self.centralwidget)
        self.serialOpenButton.setObjectName(u"serialOpenButton")
        self.serialOpenButton.setEnabled(True)

        self.horizontalLayout.addWidget(self.serialOpenButton)

        self.serialCloseButton = QPushButton(self.centralwidget)
        self.serialCloseButton.setObjectName(u"serialCloseButton")

        self.horizontalLayout.addWidget(self.serialCloseButton)

        self.serialComboBox = QComboBox(self.centralwidget)
        self.serialComboBox.setObjectName(u"serialComboBox")
        self.serialComboBox.setMinimumSize(QSize(75, 0))

        self.horizontalLayout.addWidget(self.serialComboBox)

        self.labelInfo = QLabel(self.centralwidget)
        self.labelInfo.setObjectName(u"labelInfo")

        self.horizontalLayout.addWidget(self.labelInfo)

        self.baudComboBox = QComboBox(self.centralwidget)
        self.baudComboBox.setObjectName(u"baudComboBox")
        self.baudComboBox.setMinimumSize(QSize(110, 0))

        self.horizontalLayout.addWidget(self.baudComboBox)

        self.horizontalSpacer = QSpacerItem(40, 20, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout.addItem(self.horizontalSpacer)


        self.verticalLayout_5.addLayout(self.horizontalLayout)


        self.horizontalLayout_5.addLayout(self.verticalLayout_5)


        self.verticalLayout.addLayout(self.horizontalLayout_5)


        self.verticalLayout_7.addLayout(self.verticalLayout)

        MainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QMenuBar(MainWindow)
        self.menubar.setObjectName(u"menubar")
        self.menubar.setGeometry(QRect(0, 0, 673, 22))
        MainWindow.setMenuBar(self.menubar)

        self.retranslateUi(MainWindow)

        QMetaObject.connectSlotsByName(MainWindow)
    # setupUi

    def retranslateUi(self, MainWindow):
        MainWindow.setWindowTitle(QCoreApplication.translate("MainWindow", u"GroundControlTerminal", None))
        self.consoleEdit.setPlainText("")
        self.sendButton.setText(QCoreApplication.translate("MainWindow", u"Send", None))
        self.messageEdit.setText("")
        self.serialOpenButton.setText(QCoreApplication.translate("MainWindow", u"Open", None))
        self.serialCloseButton.setText(QCoreApplication.translate("MainWindow", u"Close", None))
        self.labelInfo.setText("")
    # retranslateUi

