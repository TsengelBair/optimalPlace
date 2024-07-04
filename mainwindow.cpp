#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Основной виджет и слой
    QWidget* mainWidget = new QWidget;
    QVBoxLayout* mainLayout = new QVBoxLayout(mainWidget);

    // Слой для лейблов
    labelsLayout = new QHBoxLayout();
    QLabel* label = new QLabel("Оптимальное место: ");
    labelBtn = new QPushButton("Назад");
    labelBtn->setFixedSize(110, 30);
    optimalPlacesLabel = new QLabel("1");
    labelsLayout->addWidget(label);
    labelsLayout->addWidget(optimalPlacesLabel);
    labelsLayout->addWidget(labelBtn);
    connect(labelBtn, &QPushButton::clicked, this, &MainWindow::onReturnBtnClicked);

    // Создание слоя с шкафчиками
    gridLayout = new QGridLayout();
    gridLayout->setSpacing(0);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    mainLayout->addLayout(labelsLayout);
    mainLayout->addLayout(gridLayout);
    setCentralWidget(mainWidget);

    createCabinets();
}

MainWindow::~MainWindow() {}

void MainWindow::createCabinets()
{
    // Верхние шкафчики (нечетные)
    for (int i = 1; i <= 99; i += 2){
        QPushButton* btn = new QPushButton(QString::number(i), this);
        cabinets.append(btn);
        // Расположение в grid
        int row = 0; // строка
        int column = (i - 1) / 2; // стоблец

        // Убираем отступы, Expanding (занимает все пространство по горизонтали и вертикали) - т.к. окно приложения имеет фикс размер
        btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        gridLayout->addWidget(btn, row, column); // добавляем кнопку с указанием строки и столбца
        connect(btn, &QPushButton::clicked, this, &MainWindow::onCabinetClicked);
    }
    // Нижние (четные)
    for (int i = 2; i <= 100; i += 2) {
        QPushButton* btn = new QPushButton(QString::number(i), this);
        cabinets.append(btn);

        int row = 1;
        int column = (i / 2) - 1;

        btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        gridLayout->addWidget(btn, row, column);
        connect(btn, &QPushButton::clicked, this, &MainWindow::onCabinetClicked);
    }
}

int MainWindow::findOptimal(const QVector<int>& chosenPlaces) const
{
    QVector<int> sortedPlaces = chosenPlaces;
    std::sort(sortedPlaces.begin(), sortedPlaces.end());

    bool hasLeft = (std::find(sortedPlaces.begin(), sortedPlaces.end(), 1) != sortedPlaces.end());
    bool hasRight = (std::find(sortedPlaces.begin(), sortedPlaces.end(), 100) != sortedPlaces.end());

    int leftDistance = 0;
    if (!hasLeft) {
        leftDistance = sortedPlaces[0] - 1;
    } else {
        leftDistance = 0;
    }

    int rightDistance = 0;
    if (!hasRight) {
        rightDistance = 100 - sortedPlaces.back();
    } else {
        rightDistance = 0;
    }

    int maxDistance = 0;
    int optimalPlace = 0;

    if (sortedPlaces.size() > 1) {
        for (int i = 1; i < sortedPlaces.size(); ++i) {
            int distance = (sortedPlaces[i] - sortedPlaces[i - 1]) / 2;
            if (distance > maxDistance) {
                maxDistance = distance;
                optimalPlace = sortedPlaces[i] - distance;
            }
        }
    } else {
        // Если в векторе только один элемент, то оптимальным будет это единственное место
        optimalPlace = sortedPlaces[0];
    }

    if (leftDistance > maxDistance && leftDistance > rightDistance) {
        return 1;
    }
    else if (rightDistance > maxDistance && rightDistance > leftDistance) {
        return 100;
    }
    else {
        return optimalPlace;
    }
}

void MainWindow::onCabinetClicked()
{
    QPushButton* clickedBtn = qobject_cast<QPushButton*>(sender());
    if (clickedBtn) {
        int number = clickedBtn->text().toInt();
        chosenPlaces.append(number);

        // Изменяем цвет кнопки
        QPalette pal = clickedBtn->palette();
        pal.setColor(QPalette::Button, QColor(Qt::green));
        clickedBtn->setAutoFillBackground(true);
        clickedBtn->setPalette(pal);
        clickedBtn->update();

        int optimalPlace = findOptimal(chosenPlaces);

        // Обновляем лейбл с оптимальным местом
        optimalPlacesLabel->setText(QString::number(optimalPlace));
    }
}

void MainWindow::onReturnBtnClicked()
{
    if (!chosenPlaces.isEmpty()) {
        int lastSelected = chosenPlaces.last();

        // Находим кнопку с последним выбранным номером
        for (QPushButton* btn : cabinets) {
            if (btn->text().toInt() == lastSelected) {
                // Сбрасываем стиль кнопки (снимаем выделение)
                QPalette pal = btn->palette();
                pal.setColor(QPalette::Button, this->palette().color(QPalette::Button));
                btn->setPalette(pal);
                btn->update();
                break;  // Нашли и обработали кнопку, выходим из цикла
            }
        }

        chosenPlaces.pop_back();
        int optimalPlace = findOptimal(chosenPlaces);
        optimalPlacesLabel->setText(QString::number(optimalPlace));
    }
}




