#include "mtlistthread.h"

mtListThread::mtListThread(QWidget *parent)
{
	
}

void mtListThread::run()
{

	RTCRtdbTblInterface rtdbTable;
	RTCRtdbRecord record;
	rtdbTable.Open(SUBSTATION_ID, 1);
	if ((!rtdbTable.IsOpen()))
		return ;
	int iRecPoint;
	int nIndex = rtdbTable.GetFirstRec(&record, iRecPoint);
	while (nIndex != DB_ERROR)
	{
		char *acBuffer = new char[1024];
		if (record.GetDataBuf(record.GetRecordSize(), acBuffer) != DB_ERROR)
		{
			
			SUBSTATION_PARA *substation_DATA = (SUBSTATION_PARA*)acBuffer;
			if (!substationIdName.keys().contains(substation_DATA->m_id))
			{
				QString name = QString::fromLocal8Bit(substation_DATA->m_alias);
				substationIdName.insert(substation_DATA->m_id, name);
			}
		}
		nIndex = rtdbTable.GetNextRec(&record, iRecPoint);
	}

		RTCRtdbTblInterface bayrtdbTable;
		RTCRtdbRecord bayrecord;

			bayrtdbTable.Open(BAY_ID, 1);
			if ((!bayrtdbTable.IsOpen()))
				return;
			int ibayRecPoint;
			
			int nbayIndex = bayrtdbTable.GetFirstRec(&bayrecord, ibayRecPoint);
			//QMessageBox::information(NULL, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("提示"), QMessageBox::Ok);
		
			while (nbayIndex != DB_ERROR)
			{
				char *acBuffer = new char[2048];

				if (bayrecord.GetDataBuf(bayrecord.GetRecordSize(), acBuffer) != DB_ERROR)
				{

				BAY_PARA *bay_DATA = (BAY_PARA*)acBuffer;
				QMap<int, QString> _map;
				_map.insert(bay_DATA->m_id, QString::fromLocal8Bit(bay_DATA->m_alias));
				gapIdName.insert(bay_DATA->m_substationid, _map);
				//QString ss = QString("%1").arg(bay_DATA->m_substationid);
				//QMessageBox::information(NULL, QString::fromLocal8Bit("提示"), ss, QMessageBox::Ok);

				}
				nbayIndex = bayrtdbTable.GetNextRec(&bayrecord, ibayRecPoint);
			}

			RTCRtdbTblInterface anglogrtdbTable;
			RTCRtdbRecord anglogrecord;
			anglogrtdbTable.Open(RTANALOG_ID, 1);
			if ((!anglogrtdbTable.IsOpen()))
				return;
			int anaiRecPoint;
			int ananIndex = anglogrtdbTable.GetFirstRec(&anglogrecord, anaiRecPoint);
			while (ananIndex != DB_ERROR)
			{
				char *acBuffer = new char[1024];
				if (anglogrecord.GetDataBuf(anglogrecord.GetRecordSize(), acBuffer) != DB_ERROR)
				{
					RTANALOG_DATA *rtanalog_DATA = (RTANALOG_DATA*)acBuffer;
					m_rtanalogList.append(rtanalog_DATA);
				}
				ananIndex = anglogrtdbTable.GetNextRec(&anglogrecord, anaiRecPoint);
			}

}
