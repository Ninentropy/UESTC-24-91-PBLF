#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "music.h"
#include "NoteList.h"
#include "MusicProcess.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QIcon>
#include <QLabel>
#include <QVBoxLayout>
#include <QSizePolicy>
#include <QScrollArea>
#include <QPixmap>
#include <QWidget>
#include <QtWidgets>
#include <QMouseEvent>
#include <QMessageBox>
#include <QRect>
#include <QPainter>
#include <QButtonGroup>
#include <QTransform>
#include <QPen>
#include <QBrush>
#include <QInputDialog>
#include <QDir>
#include <QString>
#include <QList>
//全局变量
MusicManager* musicManagerPtr = new MusicManager();
NoteList* NoteListPtr    = new NoteList();
MusicPlayer* MusicPlayerPtr    = new MusicPlayer();
MusicDeserializer* MusicDeserializerPtr    = new MusicDeserializer();
std::vector<NoteList> noteTable;
//音符//八分变形音符//升降号

Note notehigh(0,0x7f);
Note notevoid(0,0);
int linecount=1;//行数量
bool eight,updownnote;
int upordown;
int kind=0;//音符时值类型
double defaultDuration=250;
double Duration=250;
std::string stdString;
QPixmap note,note2,note3,up,down;
class KindChangedSignal : public QObject
{
    Q_OBJECT
signals:
    void kindChanged(int newKind);
};
class ClickableLabel : public QLabel {
    Q_OBJECT

public:
    ClickableLabel(QWidget *parent = nullptr) : QLabel(parent) {

    }

protected:
    void mousePressEvent(QMouseEvent *event) override {
        // 获取当前鼠标点击的坐标
        clickPos = event->pos();
        qDebug() << "Mouse click position: " << clickPos;
        QLabel *newLabel = new QLabel(this);
        QLabel *notesign = new QLabel(this);
         if (kind==0) {
        newLabel->setFixedHeight(65);
         }else if (kind==1){
        newLabel->setFixedHeight(40);
         }else if (kind==2){
        newLabel->setFixedHeight(25);
         }

        newLabel->setAlignment(Qt::AlignCenter);
         QTransform upsidedown;
         upsidedown.rotate(180);
        down = note.scaled(newLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        up = note.scaled(newLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        down=down.transformed(upsidedown);
        QRect D5(0, 0, 1024, 14), C5(0, 14, 1024, 8), B4(0, 22, 1024, 8), A4(0, 30, 1024, 8), G4(0, 38, 1024, 8),
            F4(0, 46, 1024, 8), E4(0, 54, 1024, 8), D4(0, 62, 1024, 8), C4(0, 70, 1024, 8), B3(0, 78, 1024, 8),
            A3(0, 86, 1024, 8), G3(0, 94, 1024, 8), F3(0, 102, 1024, 8), E3(0, 110, 1024, 8), D3(0, 118, 1024, 8),
            C3(0, 126, 1024, 8), B2(0, 134, 1024, 8), A2(0, 142, 1024, 8), G2(0, 150, 1024, 10),

            N1(185,0,70,160),N2(255,0,70,160),N3(325,0,70,160),N4(395,0,70,160),N5(465,0,70,160),
            N6(535,0,70,160),N7(605,0,70,160),N8(675,0,70,160),N9(755,0,70,160),N10(825,0,70,160);
        if(kind==0){

        if        (D5.contains(clickPos)) {
            if(eight){down = note2.scaled(newLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);}
            QPainter painter(&down);
            QPen pen;
            pen.setColor(Qt::black);  // 设置线条颜色
            pen.setWidth(2);        // 设置线条宽度
            painter.setPen(pen);
            painter.drawLine(0, 16, 25, 16);
            painter.drawLine(0, 32, 25, 32);
            painter.end();
            newLabel->setPixmap(down);
            newLabel->move(0,1);
            notehigh.pitch=74;
        } else if (C5.contains(clickPos)) {
            if(eight){down = note2.scaled(newLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);}
            QPainter painter(&down);
            QPen pen;
            pen.setColor(Qt::black);  // 设置线条颜色
            pen.setWidth(2);        // 设置线条宽度
            painter.setPen(pen);
            painter.drawLine(0, 8, 25, 8);
            painter.drawLine(0, 24, 25, 24);
            painter.end();
            newLabel->setPixmap(down);
            newLabel->move(0,9);
            notehigh.pitch=72;
        } else if (B4.contains(clickPos)){
            if(eight){down = note2.scaled(newLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);}
            QPainter painter(&down);
            QPen pen;
            pen.setColor(Qt::black);  // 设置线条颜色
            pen.setWidth(2);        // 设置线条宽度
            painter.setPen(pen);
            painter.drawLine(0, 16, 25, 16);
            painter.end();
            newLabel->setPixmap(down);
            newLabel->move(0,17);
            notehigh.pitch=71;
        } else if (A4.contains(clickPos)){
            if(eight){down = note2.scaled(newLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);}
            QPainter painter(&down);
            QPen pen;
            pen.setColor(Qt::black);  // 设置线条颜色
            pen.setWidth(2);        // 设置线条宽度
            painter.setPen(pen);
            painter.drawLine(0, 8, 25, 8);
            painter.end();
            newLabel->setPixmap(down);
            newLabel->move(0,25);
            notehigh.pitch=69;
        } else if (G4.contains(clickPos)){
            if(eight){down = note2.scaled(newLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);}
            newLabel->setPixmap(down);
            newLabel->move(0,33);
            notehigh.pitch=67;
        } else if (F4.contains(clickPos)){
            if(eight){down = note2.scaled(newLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);}
            newLabel->setPixmap(down);
            newLabel->move(0,41);//基准
            notehigh.pitch=65;
        } else if (E4.contains(clickPos)){
            if(eight){down = note2.scaled(newLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);}
            newLabel->setPixmap(down);
            newLabel->move(0,49);//基准
            notehigh.pitch=64;
        } else if (D4.contains(clickPos)){
            if(eight){down = note2.scaled(newLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);}
            newLabel->setPixmap(down);
            newLabel->move(0,57);
            notehigh.pitch=62;
        } else if (C4.contains(clickPos)){
            if(eight){down = note2.scaled(newLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);}
            newLabel->setPixmap(down);
            newLabel->move(0,65);
            notehigh.pitch=60;
        } else if (B3.contains(clickPos)){
            if(eight){down = note2.scaled(newLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);}
            newLabel->setPixmap(down);
            newLabel->move(0,73);
            notehigh.pitch=59;
        } else if (A3.contains(clickPos)){
            newLabel->setPixmap(up);
            newLabel->move(0,33);
            notehigh.pitch=57;
        } else if (G3.contains(clickPos)){
            newLabel->setPixmap(up);
            newLabel->move(0,41);
            notehigh.pitch=55;
        } else if (F3.contains(clickPos)){
            newLabel->setPixmap(up);
            newLabel->move(0,49);
            notehigh.pitch=53;
        } else if (E3.contains(clickPos)){
            newLabel->setPixmap(up);
            newLabel->move(0,57);
            notehigh.pitch=52;
        } else if (D3.contains(clickPos)){
            newLabel->setPixmap(up);
            newLabel->move(0,65);
            notehigh.pitch=50;
        } else if (C3.contains(clickPos)){
            QPainter painter(&up);
            QPen pen;
            pen.setColor(Qt::black);  // 设置线条颜色
            pen.setWidth(2);        // 设置线条宽度
            painter.setPen(pen);
            painter.drawLine(0, 57, 25, 57);
            painter.end();
            newLabel->setPixmap(up);
            newLabel->move(0,73);
            notehigh.pitch=48;
        } else if (B2.contains(clickPos)){
            QPainter painter(&up);
            QPen pen;
            pen.setColor(Qt::black);  // 设置线条颜色
            pen.setWidth(2);        // 设置线条宽度
            painter.setPen(pen);
            painter.drawLine(0, 49, 25, 49);
            painter.end();
            newLabel->setPixmap(up);
            newLabel->move(0,81);
            notehigh.pitch=47;
        } else if (A2.contains(clickPos)){
            QPainter painter(&up);
            QPen pen;
            pen.setColor(Qt::black);  // 设置线条颜色
            pen.setWidth(2);        // 设置线条宽度
            painter.setPen(pen);
            painter.drawLine(0, 41, 25, 41);
            painter.drawLine(0, 57, 25, 57);
            painter.end();
            newLabel->setPixmap(up);
            newLabel->move(0,89);
            notehigh.pitch=45;
        } else if (G2.contains(clickPos)){
            QPainter painter(&up);
            QPen pen;
            pen.setColor(Qt::black);  // 设置线条颜色
            pen.setWidth(2);        // 设置线条宽度
            painter.setPen(pen);
            painter.drawLine(0, 33, 25, 33);
            painter.drawLine(0, 49, 25, 49);
            painter.end();
            newLabel->setPixmap(up);
            newLabel->move(0,95);
            notehigh.pitch=43;
        } else{
            emit areaClicked("wrong");
        }}else if(kind==1){
            newLabel->setPixmap(up);
            newLabel->move(0,55);
            notehigh.pitch=0;
        } else if(kind==2){
            newLabel->setPixmap(up);
            newLabel->move(0,65);
            notehigh.pitch=0;
        }
        if(upordown==1){notehigh.pitch++;}else if(upordown==2){notehigh.pitch--;}
        if (N1.contains(clickPos)){
            noteTable[0].append(notehigh);
            noteTable[0].setDuration(Duration);
            newLabel->move(210,newLabel->y());
        } else if(N2.contains(clickPos)){
            noteTable[1].append(notehigh);
            noteTable[1].setDuration(Duration);
            newLabel->move(280,newLabel->y());
        } else if(N3.contains(clickPos)){
            noteTable[2].append(notehigh);
            noteTable[2].setDuration(Duration);
            newLabel->move(350,newLabel->y());
        } else if(N4.contains(clickPos)){
            noteTable[3].append(notehigh);
            noteTable[3].setDuration(Duration);
            newLabel->move(420,newLabel->y());
        } else if(N5.contains(clickPos)){
            noteTable[4].append(notehigh);
            noteTable[4].setDuration(Duration);
            newLabel->move(490,newLabel->y());
        } else if(N6.contains(clickPos)){
            noteTable[5].append(notehigh);
            noteTable[5].setDuration(Duration);
            newLabel->move(560,newLabel->y());
        } else if(N7.contains(clickPos)){
            noteTable[6].append(notehigh);
            noteTable[6].setDuration(Duration);
            newLabel->move(630,newLabel->y());
        } else if(N8.contains(clickPos)){
            noteTable[7].append(notehigh);
            noteTable[7].setDuration(Duration);
            newLabel->move(700,newLabel->y());
        } else if(N9.contains(clickPos)){
            noteTable[8].append(notehigh);
            noteTable[8].setDuration(Duration);
            newLabel->move(770,newLabel->y());
        } else if(N10.contains(clickPos)){
            noteTable[9].append(notehigh);
            noteTable[9].setDuration(Duration);
            newLabel->move(840,newLabel->y());
        } else{
            newLabel->clear();
        }
        if(updownnote&&kind==0){
            QPixmap updown;
            notesign->setAlignment(Qt::AlignCenter);
            notesign->setFixedHeight(30);
            updown = note3.scaled(notesign->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
            notesign->setPixmap(updown);
            if(clickPos.y()<86){
            notesign->move(newLabel->x()-17,newLabel->y()-7);
            } else{
            notesign->move(newLabel->x()-17,newLabel->y()+43);
            }
            notesign->show();
        }

        newLabel->show();

    }

signals:
    void areaClicked(const QString &message);

private:

    QPoint clickPos;

};

void MainWindow::on_note1_clicked(bool checked)
{if (checked) {note.load(":/images/images/n1.png");kind=0;eight=false;Duration=defaultDuration*4;}}
void MainWindow::on_note2_clicked(bool checked)
{if (checked) {note.load(":/images/images/n2.png");kind=0;eight=false;Duration=defaultDuration*2;}}
void MainWindow::on_note4_clicked(bool checked)
{if (checked) {note.load(":/images/images/n4.png");kind=0;eight=false;Duration=defaultDuration;}}
void MainWindow::on_note8_clicked(bool checked)
{if (checked) {note.load(":/images/images/n8.png");kind=0;note2.load(":/images/images/2n8.png");eight=true;}Duration=defaultDuration/2;}
void MainWindow::on_pause4_clicked(bool checked)
{if (checked) {note.load(":/images/images/n4pause.png");kind=1;eight=false;Duration=defaultDuration;}}
void MainWindow::on_pause8_clicked(bool checked)
{if (checked) {note.load(":/images/images/n8pause.png");kind=2;eight=false;Duration=defaultDuration/2;}}
void MainWindow::on_unk_clicked(bool notechecked)
{if (notechecked){updownnote=true;upordown=1;note3.load(":/images/images/notedown.png");ui->unk_2->setChecked(false);}else{updownnote=false;upordown=0;ui->unk->setChecked(false);}}
void MainWindow::on_unk_2_clicked(bool notechecked)
{if (notechecked){updownnote=true;upordown=2;note3.load(":/images/images/noteup.png");ui->unk->setChecked(false);}else{updownnote=false;upordown=0;ui->unk_2->setChecked(false);}}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    QButtonGroup *buttonGroup = new QButtonGroup(this);
    buttonGroup->setExclusive(true);  // 确保只有一个按钮可以被选中
    // 将按钮添加到按钮组中
    buttonGroup->addButton(ui->note8);
    buttonGroup->addButton(ui->note4);
    buttonGroup->addButton(ui->note2);
    buttonGroup->addButton(ui->note1);
    buttonGroup->addButton(ui->pause4);
    buttonGroup->addButton(ui->pause8);

    ui->note8->setCheckable(true);
    ui->note4->setCheckable(true);
    ui->note2->setCheckable(true);
    ui->note1->setCheckable(true);
    ui->pause4->setCheckable(true);//四分休止
    ui->pause8->setCheckable(true);//二分休止
    ui->unk->setCheckable(true);
    ui->unk_2->setCheckable(true);

    ui->note8->setChecked(false);  // 默认按钮为未按下状态
    ui->note4->setChecked(true);
    ui->note2->setChecked(false);
    ui->note1->setChecked(false);
    ui->pause4->setChecked(false);
    ui->pause8->setChecked(false);
    ui->unk->setChecked(false);
    ui->unk_2->setChecked(false);
// 默认按钮为未按下状态
    QIcon icon(":/images/images/music.ico");  // 使用 Qt 资源文件中的图标路径
    setWindowIcon(icon);  // 设置窗口图标


    QDir songDir("../songs");
    // 设置过滤器，只获取.txt文件
    QStringList filters;
    filters << "*.txt";
    songDir.setNameFilters(filters);

    // 获取符合过滤器条件的文件列表（即所有.txt文件）
    QFileInfoList fileList = songDir.entryInfoList();

    // 遍历文件列表，为每个.txt文件创建一个QListWidgetItem并添加到QListWidget中
    for (const QFileInfo& fileInfo : fileList) {
        QString fileName = fileInfo.fileName();
        fileName.remove(".txt");
        QListWidgetItem* item = new QListWidgetItem(fileName);
        item->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        ui->listWidget->addItem(item);
    }

    QListWidgetItem* itemplus = new QListWidgetItem("新建乐谱");
    itemplus->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    ui->listWidget->addItem(itemplus);
    connect(ui->listWidget, &QListWidget::itemClicked, this, &MainWindow::onItemClicked);
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_New_triggered(){
    qDebug() << "on_New_triggered function is called.";
    bool ok;
    QString text = QInputDialog::getText(nullptr, "命名", "请输入乐曲名称:", QLineEdit::Normal, "", &ok);
    if (ok && !text.isEmpty()) {
        QListWidgetItem* itemplus = new QListWidgetItem(text);
        itemplus->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        ui->listWidget->insertItem(ui->listWidget->count() - 1, itemplus);
        stdString = text.toStdString();
        qDebug() <<  stdString;
        musicManagerPtr->create(stdString);
        ui->listWidget->show();
    }
}
void MainWindow::on_Delete_triggered(){
qDebug() << "on_Delete_triggered function is called.";
    bool ok;
    QListWidgetItem* item = nullptr;
    QString text = QInputDialog::getText(nullptr, "搜索", "请输入你要删除的乐曲名称:", QLineEdit::Normal, "",&ok);
    if (ok) {
        int itemCount = ui->listWidget->count();
        for (int i = 0; i < itemCount; i++) {
            QListWidgetItem* Sitem = ui->listWidget->item(i);
            QString itemText = Sitem->text();
            if (itemText == text ) {
                stdString = text.toStdString();
                musicManagerPtr->remove(stdString);
                QMessageBox::information(nullptr,"结果","删除成功");
                QListWidget* listWidget = ui->listWidget;
                listWidget->clear();
                QDir songDir ("../songs");
                QStringList filters;
                filters << "*.txt";
                songDir.setNameFilters(filters);
                // 获取符合过滤器条件的文件列表（即所有.txt文件）
                QFileInfoList fileList = songDir.entryInfoList();
                for (const QFileInfo& fileInfo : fileList) {
                    QString fileName = fileInfo.fileName();
                    fileName.remove(".txt");
                    QListWidgetItem* item = new QListWidgetItem(fileName);
                    item->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
                    ui->listWidget->addItem(item);
                }

                QListWidgetItem* itemplus = new QListWidgetItem("新建乐谱");
                itemplus->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
                ui->listWidget->addItem(itemplus);
                connect(ui->listWidget, &QListWidget::itemClicked, this, &MainWindow::onItemClicked);
                return;
            }
        }
        if(item != nullptr){MainWindow::onItemClicked(item);
        }else{
            QMessageBox::information(nullptr,"搜索结果","文件不存在");
        }
    }
}
void MainWindow::on_Search_triggered(){
    qDebug() << "on_Search_triggered function is called.";
    bool ok;
    QListWidgetItem* item = nullptr;
    QString text = QInputDialog::getText(nullptr, "搜索", "请输入你要打开的乐曲名称:", QLineEdit::Normal, "",&ok);
    if (ok) {
        int itemCount = ui->listWidget->count();
        for (int i = 0; i < itemCount; i++) {
            QListWidgetItem* Sitem = ui->listWidget->item(i);
            QString itemText = Sitem->text();
            if (itemText == text ) {
                stdString = text.toStdString();
               item = ui->listWidget->item(i);
            }
        }
        if(item != nullptr){MainWindow::onItemClicked(item);
        }else{
            QMessageBox::information(nullptr,"搜索结果","文件不存在");
        }
    }
}

void MainWindow::on_newline_clicked()
{
    linecount++;
    QLabel *newLabel = new QLabel(this);
    QPixmap pixmap(":/images/images/line.png");
    newLabel->setFixedSize(1024, 160);
    QSize labelSize = newLabel->size();
    pixmap = pixmap.scaled(labelSize, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    newLabel->setPixmap(pixmap);
    ClickableLabel *clickableLabel = new ClickableLabel(this);
    clickableLabel->setPixmap(pixmap);
    clickableLabel->setFixedSize(1024, 160);
    QWidget *scrollAreaWidgetContents = ui->scrollAreaWidgetContents;
    QVBoxLayout *layout = qobject_cast<QVBoxLayout *>(scrollAreaWidgetContents->layout());
    layout->insertWidget(linecount-2, clickableLabel);
    layout->setSpacing(0);
    note.load(":/images/images/n4.png");
    ui->scrollArea->setWidget(scrollAreaWidgetContents);

    if(linecount>2){
        for(int i=0;i < 10;i++){
            if(noteTable[i].isEmpty()){noteTable[i].append(notevoid);}
        }
        musicManagerPtr->saveLine(noteTable,defaultDuration,true);NoteListPtr->clear();}
        for(int i=0;i < noteTable.size();i++){
            noteTable[i].display();
            noteTable[i].clear();
    }
    noteTable.resize(10);
}
#include "mainwindow.moc"


void MainWindow::on_replay_clicked()
{
    QWidget *scrollAreaWidgetContents = ui->scrollAreaWidgetContents;
    removeAllQLabelsRecursively(scrollAreaWidgetContents);
    linecount=1;
    noteTable.clear();
}
void MainWindow::removeAllQLabelsRecursively(QWidget *parent)
{
    musicManagerPtr->remove(stdString);
    musicManagerPtr->create(stdString);
    QList<QLabel *> labelsToDelete;
    // 遍历父控件的子控件
    for (QObject *child : parent->children()) {
        QWidget *widget = qobject_cast<QWidget *>(child);
        if (widget) {
            // 如果是 QLabel，则添加到待删除列表
            QLabel *label = qobject_cast<QLabel *>(widget);
            if (label) {
                labelsToDelete.append(label);
            }
            // 如果控件有布局，递归处理
            if (widget->layout()) {
                removeAllQLabelsRecursively(widget);
            }
        }
    }
    // 删除所有收集到的 QLabel
    for (QLabel *label : labelsToDelete) {
        // 从父控件的布局中移除 QLabel
        if (label->parentWidget()) {
            QLayout *layout = label->parentWidget()->layout();
            if (layout) {
                layout->removeWidget(label);
            }
        }
        // 删除 QLabel
        delete label;
    }
}

void MainWindow::onItemClicked(QListWidgetItem* item)
{


    stdString = item->text().toStdString();
    // 判断当前点击的是哪个Item
    if (item == ui->listWidget->item(ui->listWidget->count()-1)) {
        // 你可以在这里写你想要执行的操作
        bool ok;
        QString text = QInputDialog::getText(nullptr, "命名", "请输入乐曲名称:", QLineEdit::Normal, "", &ok);
        if (ok && !text.isEmpty()) {
            QListWidgetItem* itemplus = new QListWidgetItem(text);
            itemplus->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
            ui->listWidget->insertItem(ui->listWidget->count() - 1, itemplus);
            stdString = text.toStdString();
            qDebug() <<  stdString;
            musicManagerPtr->create(stdString);
            ui->listWidget->show();
        }
    }else if (item->text() == stdString && !item->text().isEmpty()) {

        ui->listWidget->hide();
        ui->curMusic->setText(item->text());
          // 创建指向MusicManager类实例的指针
        musicManagerPtr->load(stdString);  // 通过指针调用load函
        for (int i = 0; i < musicManagerPtr->currentMusicList.size(); i++)
        {
            linecount++;
            QLabel *newLabel = new QLabel(this);
            QPixmap pixmap(":/images/images/line.png");
            newLabel->setFixedSize(1024, 160);
            QSize labelSize = newLabel->size();
            pixmap = pixmap.scaled(labelSize, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
            newLabel->setPixmap(pixmap);
            ClickableLabel *clickableLabel = new ClickableLabel(this);
            clickableLabel->setPixmap(pixmap);
            clickableLabel->setFixedSize(1024, 160);
            QWidget *scrollAreaWidgetContents = ui->scrollAreaWidgetContents;
            QVBoxLayout *layout = qobject_cast<QVBoxLayout *>(scrollAreaWidgetContents->layout());
            layout->insertWidget(linecount-2, clickableLabel);
            layout->setSpacing(0);
            note.load(":/images/images/n4.png");
            ui->scrollArea->setWidget(scrollAreaWidgetContents);

            std::string musicStr = musicManagerPtr->currentMusicList.findAt(i);// 打印一行
            std::vector<NoteList> deserializedResult = MusicDeserializerPtr->deserialize(musicStr, Duration);
            // MainWindow::on_newline_clicked();

            for (int j = 0; j < deserializedResult.size(); j++)// 打印每组
            {
                qDebug()<<deserializedResult[j].getDuration();
                note.load(":/images/images/n4.png");kind=0;eight=false;
                    if(Duration != deserializedResult[j].getDuration()){
                        if(Duration < deserializedResult[j].getDuration()){
                            if(Duration*2.05 < deserializedResult[j].getDuration()){
                                note.load(":/images/images/n1.png");kind=0;eight=false;
                            }
                            else{
                                note.load(":/images/images/n2.png");kind=0;eight=false;
                            }
                        } else{
                            note.load(":/images/images/n8.png");kind=0;eight=true;note2.load(":/images/images/2n8.png");
                        }
                    }
                for(int k = 0;k<deserializedResult[j].size();k++)//打印每个
                {
                    qDebug()<< "index"<<k<<deserializedResult[j].findAt(k).pitch;
                    QLabel *newLabel = new QLabel(ui->scrollAreaWidgetContents->layout()->itemAt(i)->widget());
                    QLabel *notesign = new QLabel(ui->scrollAreaWidgetContents->layout()->itemAt(i)->widget());
                    if (kind==0) {
                        newLabel->setFixedHeight(65);
                    }else if (kind==1){
                        newLabel->setFixedHeight(40);
                    } else if (kind==2){
                        newLabel->setFixedHeight(25);
                    }
                        newLabel->setAlignment(Qt::AlignCenter);
                        QTransform upsidedown;
                        upsidedown.rotate(180);
                        down = note.scaled(newLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
                        up = note.scaled(newLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
                        down=down.transformed(upsidedown);
                        switch (deserializedResult[j].findAt(k).pitch){
                        case 74 :{
                            if(eight){down = note2.scaled(newLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);}
                            QPainter painter(&down);
                            QPen pen;
                            pen.setColor(Qt::black);
                            pen.setWidth(2);
                            painter.setPen(pen);
                            painter.drawLine(0, 16, 25, 16);
                            painter.drawLine(0, 32, 25, 32);
                            painter.end();
                            newLabel->setPixmap(down);
                            newLabel->move(210+70*j,1);}break;
                        case 73 :{
                            if(eight){down = note2.scaled(newLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);}
                            QPainter painter(&down);
                            QPen pen;
                            pen.setColor(Qt::black);
                            pen.setWidth(2);
                            painter.setPen(pen);
                            painter.drawLine(0, 8, 25, 8);
                            painter.drawLine(0, 24, 25, 24);
                            painter.end();
                            newLabel->setPixmap(down);
                            newLabel->move(210+70*j,9);
                            QPixmap updown;
                            notesign->setAlignment(Qt::AlignCenter);
                            notesign->setFixedHeight(30);
                            updown = note3.scaled(notesign->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
                            notesign->setPixmap(updown);
                            notesign->move(newLabel->x()-17,newLabel->y()-7);
                            notesign->show();}
                        case 72 : {
                            if(eight){down = note2.scaled(newLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);}
                            QPainter painter(&down);
                            QPen pen;
                            pen.setColor(Qt::black);
                            pen.setWidth(2);
                            painter.setPen(pen);
                            painter.drawLine(0, 8, 25, 8);
                            painter.drawLine(0, 24, 25, 24);
                            painter.end();
                            newLabel->setPixmap(down);
                            newLabel->move(210+70*j,9);break;}
                        case 71 : {
                            if(eight){down = note2.scaled(newLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);}
                            QPainter painter(&down);
                            QPen pen;
                            pen.setColor(Qt::black);
                            pen.setWidth(2);
                            painter.setPen(pen);
                            painter.drawLine(0, 16, 25, 16);
                            painter.end();
                            newLabel->setPixmap(down);
                            newLabel->move(210+70*j,17);}break;
                        case 70 :{
                            if(eight){down = note2.scaled(newLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);}
                            QPainter painter(&down);
                            QPen pen;
                            pen.setColor(Qt::black);
                            pen.setWidth(2);
                            painter.setPen(pen);
                            painter.drawLine(0, 8, 25, 8);
                            painter.end();
                            newLabel->setPixmap(down);
                            newLabel->move(210+70*j,25);
                            QPixmap updown;
                            notesign->setAlignment(Qt::AlignCenter);
                            notesign->setFixedHeight(30);
                            updown = note3.scaled(notesign->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
                            notesign->setPixmap(updown);
                            notesign->move(newLabel->x()-17,newLabel->y()-7);
                            notesign->show();}break;
                        case 69 :{
                            if(eight){down = note2.scaled(newLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);}
                            QPainter painter(&down);
                            QPen pen;
                            pen.setColor(Qt::black);
                            pen.setWidth(2);
                            painter.setPen(pen);
                            painter.drawLine(0, 8, 25, 8);
                            painter.end();
                            newLabel->setPixmap(down);
                            newLabel->move(210+70*j,25);}break;
                        case 68 :{
                            if(eight){down = note2.scaled(newLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);}
                            newLabel->setPixmap(down);
                            newLabel->move(210+70*j,33);
                            QPixmap updown;
                            notesign->setAlignment(Qt::AlignCenter);
                            notesign->setFixedHeight(30);
                            updown = note3.scaled(notesign->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
                            notesign->setPixmap(updown);
                            notesign->move(newLabel->x()-17,newLabel->y()-7);
                            notesign->show();}break;
                        case 67 :{
                            if(eight){down = note2.scaled(newLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);}
                            newLabel->setPixmap(down);
                            newLabel->move(210+70*j,33);}break;
                        case 66 :{
                            if(eight){down = note2.scaled(newLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);}
                            newLabel->setPixmap(down);
                            newLabel->move(210+70*j,41);
                            QPixmap updown;
                            notesign->setAlignment(Qt::AlignCenter);
                            notesign->setFixedHeight(30);
                            updown = note3.scaled(notesign->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
                            notesign->setPixmap(updown);
                            notesign->move(newLabel->x()-17,newLabel->y()-7);
                            notesign->show();}break;
                        case 65 :{
                            if(eight){down = note2.scaled(newLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);}
                            newLabel->setPixmap(down);
                            newLabel->move(210+70*j,41);}break;
                        case 64 :{
                            if(eight){down = note2.scaled(newLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);}
                            newLabel->setPixmap(down);
                            newLabel->move(210+70*j,49);}break;
                        case 63 :{
                            if(eight){down = note2.scaled(newLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);}
                            newLabel->setPixmap(down);
                            newLabel->move(210+70*j,57);
                            QPixmap updown;
                            notesign->setAlignment(Qt::AlignCenter);
                            notesign->setFixedHeight(30);
                            updown = note3.scaled(notesign->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
                            notesign->setPixmap(updown);
                            notesign->move(newLabel->x()-17,newLabel->y()-7);
                            notesign->show();}break;
                        case 62 :{
                            if(eight){down = note2.scaled(newLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);}
                            newLabel->setPixmap(down);
                            newLabel->move(210+70*j,57);}break;
                        case 61 :{
                            if(eight){down = note2.scaled(newLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);}
                            newLabel->setPixmap(down);
                            newLabel->move(210+70*j,65);
                            QPixmap updown;
                            notesign->setAlignment(Qt::AlignCenter);
                            notesign->setFixedHeight(30);
                            updown = note3.scaled(notesign->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
                            notesign->setPixmap(updown);
                            notesign->move(newLabel->x()-17,newLabel->y()-7);
                            notesign->show();}break;
                        case 60 :{
                            if(eight){down = note2.scaled(newLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);}
                            newLabel->setPixmap(down);
                            newLabel->move(210+70*j,65);}break;
                        case 59:{
                            if(eight){down = note2.scaled(newLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);}
                            newLabel->setPixmap(down);
                            newLabel->move(210+70*j,73);}break;
                        case 58 :{
                            if(eight){down = note2.scaled(newLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);}
                            newLabel->setPixmap(up);
                            newLabel->move(210+70*j,33);
                            QPixmap updown;
                            notesign->setAlignment(Qt::AlignCenter);
                            notesign->setFixedHeight(30);
                            updown = note3.scaled(notesign->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
                            notesign->setPixmap(updown);
                            notesign->move(newLabel->x()-17,newLabel->y()+43);
                            notesign->show();}break;
                        case 57 :{
                            newLabel->setPixmap(up);
                            newLabel->move(210+70*j,33);}break;
                        case 56 :{
                            if(eight){down = note2.scaled(newLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);}
                            newLabel->setPixmap(up);
                            newLabel->move(210+70*j,41);
                            QPixmap updown;
                            notesign->setAlignment(Qt::AlignCenter);
                            notesign->setFixedHeight(30);
                            updown = note3.scaled(notesign->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
                            notesign->setPixmap(updown);
                            notesign->move(newLabel->x()-17,newLabel->y()+43);
                            notesign->show();}break;
                        case 55 :{
                            newLabel->setPixmap(up);
                            newLabel->move(210+70*j,41);}break;
                        case 54 :{
                            if(eight){down = note2.scaled(newLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);}
                            newLabel->setPixmap(up);
                            newLabel->move(210+70*j,49);
                            QPixmap updown;
                            notesign->setAlignment(Qt::AlignCenter);
                            notesign->setFixedHeight(30);
                            updown = note3.scaled(notesign->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
                            notesign->setPixmap(updown);
                            notesign->move(newLabel->x()-17,newLabel->y()+43);
                            notesign->show();}break;
                        case 53 :{
                            newLabel->setPixmap(up);
                            newLabel->move(210+70*j,49);}break;
                        case 52 :{
                            newLabel->setPixmap(up);
                            newLabel->move(210+70*j,57);}break;
                        case 51 :{
                            if(eight){down = note2.scaled(newLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);}
                            newLabel->setPixmap(up);
                            newLabel->move(210+70*j,65);
                            QPixmap updown;
                            notesign->setAlignment(Qt::AlignCenter);
                            notesign->setFixedHeight(30);
                            updown = note3.scaled(notesign->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
                            notesign->setPixmap(updown);
                            notesign->move(newLabel->x()-17,newLabel->y()+43);
                            notesign->show();}break;
                        case 50 :{
                            newLabel->setPixmap(up);
                            newLabel->move(210+70*j,65);}break;
                        case 49 :{
                            QPainter painter(&up);
                            QPen pen;
                            pen.setColor(Qt::black);
                            pen.setWidth(2);
                            painter.setPen(pen);
                            painter.drawLine(0, 57, 25, 57);
                            painter.end();
                            newLabel->setPixmap(up);
                            newLabel->move(210+70*j,73);
                            QPixmap updown;
                            notesign->setAlignment(Qt::AlignCenter);
                            notesign->setFixedHeight(30);
                            updown = note3.scaled(notesign->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
                            notesign->setPixmap(updown);
                            notesign->move(newLabel->x()-17,newLabel->y()+43);
                            notesign->show();}break;
                        case 48 :{
                            QPainter painter(&up);
                            QPen pen;
                            pen.setColor(Qt::black);
                            pen.setWidth(2);
                            painter.setPen(pen);
                            painter.drawLine(0, 57, 25, 57);
                            painter.end();
                            newLabel->setPixmap(up);
                            newLabel->move(210+70*j,73);}break;
                        case 47 :{
                            QPainter painter(&up);
                            QPen pen;
                            pen.setColor(Qt::black);
                            pen.setWidth(2);
                            painter.setPen(pen);
                            painter.drawLine(0, 49, 25, 49);
                            painter.end();
                            newLabel->setPixmap(up);
                            newLabel->move(210+70*j,81);}break;
                        case 46 :{
                            QPainter painter(&up);
                            QPen pen;
                            pen.setColor(Qt::black);
                            pen.setWidth(2);
                            painter.setPen(pen);
                            painter.drawLine(0, 57, 25, 57);
                            painter.end();
                            newLabel->setPixmap(up);
                            newLabel->move(210+70*j,89);
                            QPixmap updown;
                            notesign->setAlignment(Qt::AlignCenter);
                            notesign->setFixedHeight(30);
                            updown = note3.scaled(notesign->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
                            notesign->setPixmap(updown);
                            notesign->move(newLabel->x()-17,newLabel->y()+43);
                            notesign->show();}break;
                        case 45 :{
                            QPainter painter(&up);
                            QPen pen;
                            pen.setColor(Qt::black);
                            pen.setWidth(2);
                            painter.setPen(pen);
                            painter.drawLine(0, 41, 25, 41);
                            painter.drawLine(0, 57, 25, 57);
                            painter.end();
                            newLabel->setPixmap(up);
                            newLabel->move(210+70*j,89);}break;
                        case 44 :{
                            QPainter painter(&up);
                            QPen pen;
                            pen.setColor(Qt::black);
                            pen.setWidth(2);
                            painter.setPen(pen);
                            painter.drawLine(0, 57, 25, 57);
                            painter.end();
                            newLabel->setPixmap(up);
                            newLabel->move(210+70*j,95);
                            QPixmap updown;
                            notesign->setAlignment(Qt::AlignCenter);
                            notesign->setFixedHeight(30);
                            updown = note3.scaled(notesign->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
                            notesign->setPixmap(updown);
                            notesign->move(newLabel->x()-17,newLabel->y()+43);
                            notesign->show();}break;
                        case 43 :{
                            QPainter painter(&up);
                            QPen pen;
                            pen.setColor(Qt::black);
                            pen.setWidth(2);
                            painter.setPen(pen);
                            painter.drawLine(0, 33, 25, 33);
                            painter.drawLine(0, 49, 25, 49);
                            painter.end();
                            newLabel->setPixmap(up);
                            newLabel->move(210+70*j,95);}break;
                    }
                    if(kind==1){
                        note.load(":/images/images/n4pause.png");
                        newLabel->setPixmap(up);
                        newLabel->move(0,55);
                    } else if(kind==2){note.load(":/images/images/n8pause.png");
                        newLabel->setPixmap(up);
                        newLabel->move(0,65);}
                }
            }
        }
    }
}

void MainWindow::on_circle_clicked()
{
    QWidget *scrollAreaWidgetContents = ui->scrollAreaWidgetContents;
    removeAllQLabelsRecursively(scrollAreaWidgetContents);
    stdString = ui->curMusic->text().toStdString();
    // musicManagerPtr->saveLine(noteTable,defaultDuration,true);NoteListPtr->clear();
    musicManagerPtr->saveMusic(stdString);
    musicManagerPtr->clear();
    linecount=1;
    noteTable.clear();
    ui->listWidget->show();
}


void MainWindow::on_play_clicked(bool checked)
{
    std::string musicStrAll;
    musicManagerPtr->saveMusic(stdString);
    musicManagerPtr->load(stdString);
    for (int i = 0; i < musicManagerPtr->currentMusicList.size(); i++)
    {
        std::string musicStr = musicManagerPtr->currentMusicList.findAt(i);
        MusicPlayerPtr->play(musicStr,"",Duration);
    }
}

