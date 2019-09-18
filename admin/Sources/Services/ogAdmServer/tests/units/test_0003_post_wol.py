import requests
import unittest

class TestPostWolMethods(unittest.TestCase):

    def setUp(self):
        self.url = 'http://localhost:8888/wol'
        self.headers = {'Authorization' : '07b3bfe728954619b58f0107ad73acc1'}
        self.json = { 'type' : 'broadcast', 'clients' : [ { 'addr' : '192.168.2.1',
            'mac' : '00AABBCCDD01' } ] }

    def test_post(self):
        returned = requests.post(self.url, headers=self.headers, json=self.json)
        self.assertEqual(returned.status_code, 200)

    def test_get(self):
        returned = requests.get(self.url, headers=self.headers)
        self.assertEqual(returned.status_code, 405)

if __name__ == '__main__':
    unittest.main()
