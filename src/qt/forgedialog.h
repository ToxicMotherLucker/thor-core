// Copyright (c) 2011-2017 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_QT_FORGEDIALOG_H
#define BITCOIN_QT_FORGEDIALOG_H

#include <qt/guiutil.h>

#include <QDialog>
#include <QHeaderView>
#include <QItemSelection>
#include <QKeyEvent>
#include <QMenu>
#include <QPoint>
#include <QVariant>

#include <pow.h>
#include <qt/qcustomplot.h>

class PlatformStyle;
class ClientModel;
class WalletModel;

namespace Ui {
    class ForgeDialog;
}

QT_BEGIN_NAMESPACE
class QModelIndex;
QT_END_NAMESPACE

extern HammerPopGraphPoint hammerPopGraph[1024*40];

class QCPAxisTickerGI : public QCPAxisTicker 
{
public:
    double global100;

    QString getTickLabel(double tick, const QLocale &locale, QChar formatChar, int precision) {
        tick = (tick / global100 * 100); // At tick = global100, scale is 100
        return QString::number((int)tick);
    }
};

class ForgeDialog : public QDialog
{
    Q_OBJECT

public:
    enum ColumnWidths {
        CREATED_COLUMN_WIDTH = 100,
        COUNT_COLUMN_WIDTH = 80,
        STATUS_COLUMN_WIDTH = 120,
        TIME_COLUMN_WIDTH = 300,
        COST_COLUMN_WIDTH = 110,
        REWARDS_COLUMN_WIDTH = 220,
        FORGE_COL_MIN_WIDTH = 100
    };

    explicit ForgeDialog(const PlatformStyle *platformStyle, QWidget *parent = 0);
    ~ForgeDialog();

    void setClientModel(ClientModel *_clientModel);
    void setModel(WalletModel *model);
    static QString formatLargeNoLocale(int i);

public Q_SLOTS:
    void updateData(bool forceGlobalSummaryUpdate = false);
    void setBalance(const CAmount& balance, const CAmount& unconfirmedBalance, const CAmount& createdBalance,
                    const CAmount& watchOnlyBalance, const CAmount& watchUnconfBalance, const CAmount& watchCreatedBalance);
    void setEncryptionStatus(int status);

Q_SIGNALS:
    void forgeStatusIconChanged(QString icon, QString tooltip);    

private:
    Ui::ForgeDialog *ui;
    GUIUtil::TableViewLastColumnResizingFixer *columnResizingFixer;
    ClientModel *clientModel;
    WalletModel *model;
    const PlatformStyle *platformStyle;
    CAmount hammerCost, totalCost;
    int created, ready, dead, blocksFound;
    CAmount rewardsPaid, cost, profit;
    CAmount potentialRewards;
    CAmount currentBalance;
    double hammerPopIndex;
    int lastGlobalCheckHeight;
    virtual void resizeEvent(QResizeEvent *event);
    QCPItemText *graphMouseoverText;
    QCPItemTracer *graphTracerReady;
    QCPItemTracer *graphTracerCreated;
    QCPItemLine *globalMarkerLine;
    QSharedPointer<QCPAxisTickerGI> giTicker;

    void updateTotalCostDisplay();
    void initGraph();
    void updateGraph();
    void showPointToolTip(QMouseEvent *event);
    void setAmountField(QLabel *field, CAmount value);

private Q_SLOTS:
    void on_showForgeOptionsButton_clicked();
    void on_createHammersButton_clicked();
    void on_hammerCountSpinner_valueChanged(int i);
    void on_includeDeadHammersCheckbox_stateChanged();
    void on_showAdvancedStatsCheckbox_stateChanged();
    void updateDisplayUnit();
    void on_retryGlobalSummaryButton_clicked();
    void on_refreshGlobalSummaryButton_clicked();
    void on_releaseSwarmButton_clicked();
    void onMouseMove(QMouseEvent* event);
};

#endif // BITCOIN_QT_FORGEDIALOG_H