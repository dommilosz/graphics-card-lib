#include "I2CCom.h"

enum
{
    RES_ZX = 0, // 256x192
    RES_CGA,    // 320x200
    RES_QVGA,   // 320x240
    RES_EGA,    // 512x400
};

struct gcard_status{
	uint8_t initialized = 0;
	uint8_t last_asset_write_finished = 0;
};

#define WriteU16ToArr(arr, index, value) arr[index] = value >> 8; arr[index+1] = value & 0xFF;



class GraphicsCardI2CCom
{
public:
    I2CCom_Master *_i2c;
    GraphicsCardI2CCom(I2CCom_Master *i2c)
    {
        _i2c = i2c;
    }

    uint8_t SendRawCmd(uint8_t *data,int length)
    {
        uint8_t addr = _i2c->GetAddrByID(30);
        if (addr > 0)
        {
            _i2c->SendAndRequestData((int8_t *)data,length,addr, 1, 1);
            return _i2c->_wire->read();
        }
        return 255;
    }

    uint8_t SendRawCmd(uint8_t d1){
        uint8_t data[1];
        data[0] = d1;
        return SendRawCmd(data,1);
    }

    uint8_t SendRawCmd(uint8_t d1, uint8_t d2){
        uint8_t data[2];
        data[0] = d1;
        data[1] = d2;
        return SendRawCmd(data,2);
    }
    uint8_t SendRawCmd(uint8_t d1, uint8_t d2,uint8_t d3){
        uint8_t data[3];
        data[0] = d1;
        data[1] = d2;
        data[1] = d3;
        return SendRawCmd(data,3);
    }
    uint8_t SendRawCmd(uint8_t d1, uint8_t d2,uint8_t d3,uint8_t d4){
        uint8_t data[4];
        data[0] = d1;
        data[1] = d2;
        data[2] = d3;
        data[3] = d4;
        return SendRawCmd(data,4);
    }
    uint8_t SendRawCmd(uint8_t d1, uint8_t d2,uint8_t d3,uint8_t d4, uint8_t d5){
        uint8_t data[5];
        data[0] = d1;
        data[1] = d2;
        data[2] = d3;
        data[3] = d4;
        data[4] = d5;
        return SendRawCmd(data,5);
    }
	
	uint8_t SendRawCmdOutput(uint8_t *data,int length, int req_len)
    {
        uint8_t addr = _i2c->GetAddrByID(30);
        if (addr > 0)
        {
            _i2c->SendAndRequestData((int8_t *)data,length,addr, 2, req_len);
            return 1;
        }
        return 0;
    }

    uint8_t Init(uint8_t resolution = 3)
    {
        return SendRawCmd(4,resolution);
    }
	
	uint8_t InitIfNot(uint8_t resolution = 3)
    {
		gcard_status status;
		Status(&status);
		if (status.initialized > 0) {
			return 0;
		}
        return SendRawCmd(4,resolution);
    }

    uint8_t Clear(uint8_t color = 0)
    {
        return SendRawCmd(3,color);
    }

    uint8_t Feature_Rectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t color){
        uint8_t data[11];
        data[0] = 6;
        data[1] = 0;
        WriteU16ToArr(data,2,x);
        WriteU16ToArr(data,4,y);
        WriteU16ToArr(data,6,w);
        WriteU16ToArr(data,8,h);
        data[10] = color;
        return SendRawCmd(data,11);
    }

    uint8_t Feature_Point(uint16_t x, uint16_t y, uint8_t color){
        uint8_t data[7];
        data[0] = 6;
        data[1] = 1;
        WriteU16ToArr(data,2,x);
        WriteU16ToArr(data,4,y);
        data[6] = color;
        return SendRawCmd(data,7);
    }

    uint8_t Feature_Line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t color){
        uint8_t data[11];
        data[0] = 6;
        data[1] = 2;
        WriteU16ToArr(data,2,x1);
        WriteU16ToArr(data,4,y1);
        WriteU16ToArr(data,6,x2);
        WriteU16ToArr(data,8,y2);
        data[10] = color;
        return SendRawCmd(data,11);
    }

    uint8_t Objects_Create(uint8_t index,uint8_t type){
        return SendRawCmd(8,index,0,type);
    }

    uint8_t Objects_Delete(uint8_t index){
        return SendRawCmd(8,index,1);
    }

    uint8_t Objects_Move(uint8_t index,uint8_t relative, int16_t x, int16_t y){
        uint8_t data[8];
        data[0] = 8;
        data[1] = index;
        data[2] = 2;
        data[3] = relative;
        WriteU16ToArr(data,4,x);
        WriteU16ToArr(data,6,y);
        return SendRawCmd(data,8);
    }

    uint8_t Objects_Color(uint8_t index, uint8_t color){
        return SendRawCmd(8,index,3,color);
    }

    uint8_t Objects_Visibility(uint8_t index, uint8_t visibility){
        return SendRawCmd(8,index,4,visibility);
    }

    uint8_t Objects_Resize(uint8_t index,uint8_t relative, int16_t w, int16_t h){
        uint8_t data[8];
        data[0] = 8;
        data[1] = index;
        data[2] = 5;
        data[3] = relative;
        WriteU16ToArr(data,4,w);
        WriteU16ToArr(data,6,h);
        return SendRawCmd(data,8);
    }

    uint8_t Objects_Radius(uint8_t index,uint8_t relative, int16_t r){
        uint8_t data[6];
        data[0] = 8;
        data[1] = index;
        data[2] = 6;
        data[3] = relative;
        WriteU16ToArr(data,4,r);
        return SendRawCmd(data,6);
    }

    uint8_t Objects_MaskFill(uint8_t index,uint8_t mask, uint8_t fill){
        return SendRawCmd(8,index,7,mask,fill);
    }

    uint8_t Objects_MoveLine(uint8_t index,uint8_t relative, int16_t x1, int16_t y1, int16_t x2, int16_t y2){
        uint8_t data[12];
        data[0] = 8;
        data[1] = index;
        data[2] = 8;
        data[3] = relative;
        WriteU16ToArr(data,4,x1);
        WriteU16ToArr(data,6,y1);
        WriteU16ToArr(data,8,x2);
        WriteU16ToArr(data,10,y2);
        return SendRawCmd(data,12);
    }

    uint8_t Objects_TextAsset(uint8_t index,uint8_t text){
        return SendRawCmd(8,index,9,text);
    }

    uint8_t Objects_CodeAsset(uint8_t index,uint8_t code){
        return SendRawCmd(8,index,11,code);
    }

    uint8_t Objects_BlobAsset(uint8_t index,uint8_t code){
        return SendRawCmd(8,index,12,code);
    }

	#define SEND_BLOCK_SIZE 120
    uint8_t WriteAsset(uint8_t asset_index, uint8_t *asset_data, uint16_t length, uint16_t offset = 0){
		if(length<=0)return 0;
		uint16_t _length = length;
		uint8_t segment_location = 254;
		if(_length>SEND_BLOCK_SIZE){
			_length = SEND_BLOCK_SIZE;
			segment_location = 1;
			if(offset == 0){
				segment_location = 0;
			}
		}else{
			segment_location = 255;
			if(offset == 0){
				segment_location = 254;
			}
		}
        uint8_t data[_length+7];
        data[0] = 11;
        data[1] = asset_index;
        WriteU16ToArr(data,2,offset);
        WriteU16ToArr(data,4,_length);
		data[6] = segment_location;
        for(uint16_t i =0;i<_length;i++){
            data[7+i] = asset_data[i+offset];
        }
		uint8_t output = SendRawCmd(data,_length+7);
        if(output==0){
			if((length-_length)>0){
				_i2c->ScanDevices();
				return WriteAsset(asset_index,asset_data,length-_length,offset+_length);
			}
			return 0;
		}
		return 2;
    }
	
	uint8_t WriteAssetOrCRC(uint8_t asset_index, uint8_t *asset_data, uint16_t length){
		uint32_t crc_read = 0;
		if(Asset_CRC32(asset_index, length, &crc_read) == 0){
			uint32_t crc_gen = crc32(asset_data,length);
			if(crc_gen == crc_read)return 0;
		}
		return WriteAsset(asset_index,asset_data,length);
	}
	
	uint8_t Asset_CRC32(uint8_t asset,uint16_t length, uint32_t *out_crc){
		uint8_t data[4];
		data[0] = 12;
		data[1] = asset;
		WriteU16ToArr(data,2,length);
		if(SendRawCmdOutput(data,4,4)){
			if(_i2c->_wire->available() >= 4){
				*out_crc = _i2c->_wire->read() << 24;
				*out_crc |= _i2c->_wire->read() << 16;
				*out_crc |= _i2c->_wire->read() << 8;
				*out_crc |= _i2c->_wire->read();
			}
			if(_i2c->_wire->available()> 0){
				return _i2c->_wire->read();
			}
		}
		return -1;
	}
	
	uint8_t Status(gcard_status *status){
		uint8_t data[1];
		data[0] = 253;
		if(SendRawCmdOutput(data,1,2)){
			if(_i2c->_wire->available()> 0){
				status->initialized = _i2c->_wire->read();
			}
			if(_i2c->_wire->available()> 0){
				status->last_asset_write_finished = _i2c->_wire->read();
			}
			if(_i2c->_wire->available()> 0){
				return _i2c->_wire->read();
			}
		}
		return -1;
	}
	
	uint32_t crc32(const uint8_t *s, size_t n) {
  uint32_t crc = 0xFFFFFFFF;

  for (size_t i = 0; i < n; i++) {
    char ch = s[i];
    for (size_t j = 0; j < 8; j++) {
      uint32_t b = (ch ^ crc) & 1;
      crc >>= 1;
      if (b) crc = crc ^ 0xEDB88320;
      ch >>= 1;
    }
  }

  return ~crc;
}
};