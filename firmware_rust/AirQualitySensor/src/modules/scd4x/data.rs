use super::SCD4xError;

const CRC8_POLYNOMIAL: u8 = 0x31;
const CRC8_INIT: u8 = 0xFF;

pub fn crc(data: &[u8]) -> u8 {
    let mut crc = CRC8_INIT;

    /* calculates 8-Bit checksum with given polynomial */
    for current_byte in data {
        crc ^= current_byte;
        for _ in 0..8 {
            if crc & 0x80 != 0 {
                crc = (crc << 1) ^ CRC8_POLYNOMIAL;
            } else {
                crc = crc << 1;
            }
        }
    }

    crc
}

pub struct WordsIterator<'a> {
    data: &'a [u8],
}

impl<'a> Iterator for WordsIterator<'a> {
    type Item = Result<u16, SCD4xError>;

    fn next(&mut self) -> Option<Self::Item> {
        if self.data.len() == 0 {
            return None;
        }

        if self.data.len() < 3 {
            return Some(Err(SCD4xError::ByteCountError));
        }

        let next = &self.data[..3];
        self.data = &self.data[3..];

        let checksum = crc(&next[..2]);
        if checksum != next[2] {
            return Some(Err(SCD4xError::ChecksumError));
        }

        Some(Ok((next[0] as u16) << 8 | next[1] as u16))
    }
}

pub fn convert_response_to_words(data: &[u8]) -> WordsIterator {
    WordsIterator { data }
}
