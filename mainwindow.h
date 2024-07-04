#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QVector>
#include <QGridLayout>
#include <QVector>
#include <QPushButton>


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    QLabel* optimalPlacesLabel; // Лейбл для отображения оптимального места
    QPushButton* labelBtn; // Кнопка назад
    QVector<int>chosenPlaces; // Вектор выбранных мест
    QHBoxLayout* labelsLayout; // слой для лейблов
    QGridLayout* gridLayout; // слой внутри которого и будут располагаться кнопки
    QVector<QPushButton*>cabinets;

    // не в private slots, т.к. нет это обычные методы класса
    void createCabinets();
    int findOptimal(const QVector<int>&chosenPlaces) const;

private slots:
    void onCabinetClicked();
    void onReturnBtnClicked();
};
#endif // MAINWINDOW_H
