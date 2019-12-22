#pragma once

#include <QFrame>
class QLabel;
#include "ProcessData.h"

class ProcessForm : public QFrame {
    Q_OBJECT
    Q_DISABLE_COPY( ProcessForm )
public:
    explicit ProcessForm( QWidget* parent = nullptr );
    void setProcessData( const ProcessData& );
    ProcessData processData() const;

    Q_SIGNAL void edit( const ProcessData& );
    Q_SIGNAL void remove( const ProcessData& );

private:
    Q_SLOT void onRun();
    Q_SLOT void onEdit();
    Q_SLOT void onRemove();

private:
    QLabel*const m_title;
    QLabel*const m_description;
    ProcessData m_data;
};
